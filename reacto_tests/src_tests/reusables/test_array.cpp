#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdint.h>

extern "C"
{
    #include <reacto/reusables/array.h>
}

TEST_GROUP(Array)
{

    array_t cut;
    int buffer[5 + 1];

    void setup ()
    {
        buffer[5] = 0;
        array_init(&cut, 5);
    }

    void teardown ()
    {
        array_deinit(&cut);
        mock().clear();
    }

    static bool compare (int element, int seed)
    {
        return element == seed;
    }

    static void freefunc (int element)
    {
        mock().actualCall("freefunc")
                .withIntParameter("element", element);
    }
};

TEST(Array, init)
{
    CHECK_EQUAL(5, cut.max);
    CHECK_EQUAL(0, cut.top);
}

TEST(Array, insert_remove_free_find)
{
    array_insert(&cut, buffer, 10);
    CHECK_EQUAL(10, buffer[0]);
    array_insert(&cut, buffer, 11);
    CHECK_EQUAL(11, buffer[1]);
    array_insert(&cut, buffer, 12);
    CHECK_EQUAL(12, buffer[2]);
    array_insert(&cut, buffer, 13);
    CHECK_EQUAL(13, buffer[3]);

    CHECK_EQUAL(4, cut.top);
    CHECK_EQUAL(false, array_full(&cut));

    array_insert(&cut, buffer, 14);
    CHECK_EQUAL(14, buffer[4]);
    CHECK_EQUAL(cut.max, cut.top);
    CHECK_EQUAL(true, array_full(&cut));

    /* Extra insert should trigger an warning */
    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Warning: Trying to insert in a full array, data loss happened.")
            .ignoreOtherParameters();
    array_insert(&cut, buffer, 14);
    CHECK_EQUAL(14, buffer[4]);
    CHECK_EQUAL(0, buffer[5]);

    /* Now we remove one from the middle */
    array_remove(&cut, buffer, 3);
    CHECK_EQUAL(10, buffer[0]);
    CHECK_EQUAL(11, buffer[1]);
    CHECK_EQUAL(12, buffer[2]);
    CHECK_EQUAL(14, buffer[3]);
    CHECK_EQUAL(0, buffer[5]);
    CHECK_EQUAL(4, cut.top);

    /* And then we remove an invalid position, nothing should happen */
    array_remove(&cut, buffer, 312);
    CHECK_EQUAL(10, buffer[0]);
    CHECK_EQUAL(11, buffer[1]);
    CHECK_EQUAL(12, buffer[2]);
    CHECK_EQUAL(14, buffer[3]);
    CHECK_EQUAL(0, buffer[5]);
    CHECK_EQUAL(4, cut.top);

    /* Now we remove another with freefunc */
    mock().expectOneCall("freefunc")
            .withParameter("element", 11);

    array_free(&cut, buffer, 1, freefunc);
    CHECK_EQUAL(10, buffer[0]);
    CHECK_EQUAL(12, buffer[1]);
    CHECK_EQUAL(14, buffer[2]);
    CHECK_EQUAL(0, buffer[5]);
    CHECK_EQUAL(3, cut.top);

    /* Lets do a little of searching */
    int pos = array_find(&cut, buffer, compare, 14);
    CHECK_EQUAL(2, pos);
    pos = array_find(&cut, buffer, compare, 23141241);
    CHECK_EQUAL(-1, pos);

    /* bye bye array! */
    array_clear(&cut);
    CHECK_EQUAL(5, cut.max);
    CHECK_EQUAL(0, cut.top);
}
