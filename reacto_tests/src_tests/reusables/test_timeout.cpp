#include <CppUTest/TestHarness.h>

extern "C"
{
    #include <reacto/reusables/time.h>
    #include <reacto/reusables/timeout.h>

    extern reacto_time_t time_now_variable;
}

TEST_GROUP(timeout)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(timeout, init_copy)
{
    timeout_t tout, tout2;
    timeout_init(&tout);
    timeout_copy(&tout2, &tout);
    CHECK_EQUAL(tout, tout2);
}

TEST(timeout, test)
{
    timeout_t tout;
    timeout_init(&tout);

    CHECK_FALSE(timeout_check(&tout, 100));
    time_now_variable += 1;
    CHECK_FALSE(timeout_check(&tout, 100));
    time_now_variable += 9;
    CHECK_FALSE(timeout_check(&tout, 100));
    time_now_variable += 40;
    CHECK_FALSE(timeout_check(&tout, 100));
    time_now_variable += 49;
    CHECK_FALSE(timeout_check(&tout, 100));
    time_now_variable += 1;
    CHECK_TRUE(timeout_check(&tout, 100));
}

TEST(timeout, periodically_test)
{
    timeout_t tout;
    timeout_init(&tout);

    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 1;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 9;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 40;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 49;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 1;
    CHECK_TRUE(timeout_check_and_reinit(&tout, 100));

    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 1;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 9;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 40;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 49;
    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
    time_now_variable += 1;
    CHECK_TRUE(timeout_check_and_reinit(&tout, 100));

    CHECK_FALSE(timeout_check_and_reinit(&tout, 100));
}

TEST(timeout, remaining_test)
{
    timeout_t tout;
    timeout_init(&tout);
    reacto_time_t now = time_now();
    CHECK_EQUAL(100, timeout_remaining(time_now(), now, 100));

    time_now_variable += 1;
    CHECK_EQUAL(99, timeout_remaining(time_now(), now, 100));
    time_now_variable += 10;
    CHECK_EQUAL(89, timeout_remaining(time_now(), now, 100));
    time_now_variable += 88;
    CHECK_EQUAL(0, timeout_remaining(time_now(), now, 100));

}

TEST(timeout, comparator_test)
{
    const reacto_time_t max = (reacto_time_t)-1;

    /* Comparator margin */
    CHECK_FALSE(timeout_check_elapsed(10, 0, 11));
    CHECK_TRUE (timeout_check_elapsed(10, 0, 10));

    /* Considering timer wrap */
    CHECK_FALSE(timeout_check_elapsed(max - 1, max - 1, 3));
    CHECK_FALSE(timeout_check_elapsed(max, max - 1, 3));
    CHECK_FALSE(timeout_check_elapsed(max + 1, max - 1, 3));
    CHECK_TRUE (timeout_check_elapsed(max + 2, max - 1, 3));
    CHECK_TRUE (timeout_check_elapsed(max + 3, max - 1, 3));
    CHECK_TRUE (timeout_check_elapsed(max + 4, max - 1, 3));
    CHECK_TRUE (timeout_check_elapsed(max + 5, max - 1, 3));
    CHECK_TRUE (timeout_check_elapsed(max + 6, max - 1, 3));

    CHECK_FALSE(timeout_check_elapsed(max - 1, max - 1, 1));
    CHECK_TRUE (timeout_check_elapsed(max, max - 1, 1));
    CHECK_TRUE (timeout_check_elapsed(max + 1, max - 1, 1));
    CHECK_TRUE (timeout_check_elapsed(max + 2, max - 1, 1));
    CHECK_TRUE (timeout_check_elapsed(max + 3, max - 1, 1));
    CHECK_TRUE (timeout_check_elapsed(max + 4, max - 1, 1));
    CHECK_TRUE (timeout_check_elapsed(max + 5, max - 1, 1));
    CHECK_TRUE (timeout_check_elapsed(max + 6, max - 1, 1));

    CHECK_FALSE(timeout_check_elapsed(max - 1, max + 1, max));
    CHECK_TRUE (timeout_check_elapsed(max, max + 1, max));

    CHECK_FALSE(timeout_check_elapsed(0xE, 0x10, max));
    CHECK_TRUE (timeout_check_elapsed(0xF, 0x10, max));

    /* Tests pass because the subtraction made in check_elapsed
     * removes the wrap effect.
     * It only works if "now" is bigger than "before", considering
     * we are dealing with time, now is bigger than before, always.
     */

    /* Elapsed 0 always return true! */
    CHECK_TRUE (timeout_check_elapsed(max - 2, max - 1, 0));
    CHECK_TRUE (timeout_check_elapsed(max - 1, max - 1, 0));

    /* Ok, what if I wanted to test now against a number in the future?
     * Simple! Transform later in before with a stupid big number and
     *  check against it! */
    reacto_time_t stupid_big_number = (max/2) + 1;
    reacto_time_t later = 0x00000010;
    reacto_time_t before = later - stupid_big_number;

    CHECK_FALSE(timeout_check_elapsed(max - 255, before, stupid_big_number));
    CHECK_FALSE(timeout_check_elapsed(max, before, stupid_big_number));
    CHECK_FALSE(timeout_check_elapsed(max + 1, before, stupid_big_number));
    CHECK_FALSE(timeout_check_elapsed(0x0F, before, stupid_big_number));
    CHECK_TRUE (timeout_check_elapsed(0x10, before, stupid_big_number));
    CHECK_TRUE (timeout_check_elapsed(0x11, before, stupid_big_number));

    /* As we are good programmers, we wrapped the algorithm in a new function */
    CHECK_FALSE(timeout_check_reached(0x10, max - 255));
    CHECK_FALSE(timeout_check_reached(0x10, max));
    CHECK_FALSE(timeout_check_reached(0x10, max + 1));
    CHECK_FALSE(timeout_check_reached(0x10, 0x0F));
    CHECK_TRUE (timeout_check_reached(0x10, 0x10));
    CHECK_TRUE (timeout_check_reached(0x10, 0x11));

    /* Wrap around whooooooooooooooooooooooooooooooo? */
}
