#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdint.h>
#include "linked_list.hpp"

TEST_GROUP(LinkedList)
{
    struct test_t
    {
        uint8_t b1;
        linked_list_t h;
        uint8_t b2;
    };

    test_t * root;
    test_t cut;

    void setup ()
    {
        root = NULL;
        cut = {0xAA, {0, 0}, 0x55};
    }

    void teardown ()
    {
        CHECK_EQUAL(0xAA, cut.b1);
        CHECK_EQUAL(0x55, cut.b2);
        mock().clear();
    }

    static bool compare (test_t * element, test_t * seed)
    {
        return element == seed;
    }

    static void freefunc (test_t * element)
    {
        element->b1 = 0xF1;
    }
};

TEST(LinkedList, init)
{
    linked_list_init(&cut, h);
    CHECK_EQUAL(0, cut.h.next);
    CHECK_EQUAL(0, cut.h.prev);
}

TEST(LinkedList, insert_next)
{
    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        linked_list_init(&cuts[i], h);
    linked_list_init(root, h);

    CHECK_EQUAL(root, linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        linked_list_insert_after(
                linked_list_last(root, h),
                &cuts[i],
                h);

    CHECK_EQUAL(11, linked_list_count(root, h));

    test_t * p = root;
    for (int i = 0; i<10; i++)
    {
        p = linked_list_next(p, h);
        CHECK_EQUAL(&cuts[i], p)
    }

    p = linked_list_next(p, h);
    CHECK_EQUAL(0, p);
}

TEST(LinkedList, remove)
{
    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        linked_list_init(&cuts[i], h);
    linked_list_init(root, h);

    CHECK_EQUAL(root, linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        linked_list_insert_after(
                linked_list_last(root, h),
                &cuts[i],
                h);

    CHECK_EQUAL(11, linked_list_count(root, h));

    test_t * p = root;
    for (int i = 0; i<10; i++)
    {
        p = linked_list_next(p, h);
        CHECK_EQUAL(&cuts[i], p)
    }

    p = linked_list_next(p, h);
    CHECK_EQUAL(0, p);

    root = linked_list_remove(&cuts[4], h);
    p = root;
    for (int i = 0; i<10; i++)
    {
        if (i == 4) continue;
        p = linked_list_next(p, h);
        CHECK_EQUAL(&cuts[i], p)
    }

    p = linked_list_next(p, h);
    CHECK_EQUAL(0, p);
}

TEST(LinkedList, remove2)
{
    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        linked_list_init(&cuts[i], h);
    linked_list_init(root, h);

    CHECK_EQUAL(root, linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        linked_list_insert_after(
                linked_list_last(root, h),
                &cuts[i],
                h);

    CHECK_EQUAL(11, linked_list_count(root, h));

    /* Root relocation */
    root = linked_list_remove(root, h);
    CHECK_EQUAL(root, &cuts[0]);

    test_t * p = &cuts[9];
    for (int i = 9; i>=0; i--)
    {
        CHECK_EQUAL(&cuts[i], p)
        p = linked_list_previous(p, h);
    }

    CHECK_EQUAL(0, p);
}

TEST(LinkedList, remove3)
{
    root = &cut;

    linked_list_init(root, h);
    CHECK_EQUAL(root, linked_list_last(root, h));

    root = linked_list_remove(root, h);
    CHECK_EQUAL(0, root);
}

TEST(LinkedList, insert_prev)
{
    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        linked_list_init(&cuts[i], h);
    linked_list_init(root, h);

    CHECK_EQUAL(root, linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        linked_list_insert_after(
                linked_list_last(root, h),
                &cuts[i],
                h);

    CHECK_EQUAL(11, linked_list_count(root, h));

    test_t * p = &cuts[9];
    for (int i = 9; i>=0; i--)
    {
        CHECK_EQUAL(&cuts[i], p)
        p = linked_list_previous(p, h);
    }

    CHECK_EQUAL(root, p);
}

TEST(LinkedList, insert_before)
{
    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        linked_list_init(&cuts[i], h);
    linked_list_init(root, h);

    CHECK_EQUAL(root, linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        linked_list_insert_before(
                linked_list_first(root, h),
                &cuts[i],
                h);

    /* root is the last node */
    CHECK_EQUAL(11, linked_list_count(root, h));

    test_t * p = linked_list_first(root, h);
    for (int i = 9; i>=0; i--)
    {
        CHECK_EQUAL(&cuts[i], p)
        p = linked_list_next(p, h);
    }

    CHECK_EQUAL(root, p);
}

TEST(LinkedList, find)
{
    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        linked_list_init(&cuts[i], h);
    linked_list_init(root, h);

    CHECK_EQUAL(root, linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        linked_list_insert_after(
                linked_list_last(root, h),
                &cuts[i],
                h);

    CHECK_EQUAL(11, linked_list_count(root, h));
    CHECK_EQUAL(&cuts[9], linked_list_find(root, h, compare, &cuts[9]));
}

TEST(LinkedList, free)
{
    test_t cuts[10];
    root = &cut;

    for (int i = 0; i<10; i++)
        linked_list_init(&cuts[i], h);
    linked_list_init(root, h);

    CHECK_EQUAL(root, linked_list_last(root, h));

    for (int i = 0; i<10; i++)
        linked_list_insert_after(
                linked_list_last(root, h),
                &cuts[i],
                h);

    CHECK_EQUAL(11, linked_list_count(root, h));
    linked_list_free(root, h, freefunc);

    for (int i = 0; i<10; i++)
    {
        CHECK_EQUAL(0xF1, cuts[i].b1);
        cuts[i].b1 = 0xAA;
    }

    CHECK_EQUAL(0xF1, root->b1);
    root->b1 = 0xAA;
}


TEST(LinkedList, cpp_ll_macros)
{
    typedef struct _AStruct
    {
        int data_field;
    } AStruct;

    AStruct as;
    int * ptr = &as.data_field;

    auto r = __ll_container_of(ptr, AStruct, data_field);
    auto r2 = __ll_container_of(ptr, __ll_typeof(as), data_field);

    STRCMP_EQUAL(typeid(&as).name(), typeid(r).name());
    STRCMP_EQUAL(typeid(&as).name(), typeid(r2).name());
    CHECK_EQUAL (&as, r);
    CHECK_EQUAL (&as, r2);
}
