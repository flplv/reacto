#include <CppUTest/TestHarness.h>

extern "C"
{
    #include <reacto/reusables/time.h>
    #include <reacto/reusables/timeout.h>
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

TEST(timeout, test)
{
	timeout_t tout = timeout_init_cpy();

	uint32_t now = time_now_ms();

	bool r = timeout_check(&tout, 100);
	while (!r)
	    r = timeout_check(&tout, 100);

	CHECK_TRUE( (now + 100) <= time_now_ms() );
}

TEST(timeout, periodically_test)
{
	timeout_t tout = timeout_init_cpy();

	uint32_t now = time_now_ms();

	while (!timeout_check_and_reinit(&tout, 100));
	CHECK_TRUE( (now + 100) <= time_now_ms() );

	while (!timeout_check_and_reinit(&tout, 100));
	CHECK_TRUE( (now + 200) <= time_now_ms() );

	while (!timeout_check_and_reinit(&tout, 100));
	CHECK_TRUE( (now + 300) <= time_now_ms() );

	while (!timeout_check_and_reinit(&tout, 100));
	CHECK_TRUE( (now + 400) <= time_now_ms() );
}

TEST(timeout, sleep_test)
{
	timeout_t tout = timeout_init_cpy();
	uint32_t now = time_now_ms();
	timeout_sleep(&tout, 100);
	timeout_sleep(&tout, 100); /* This one should not wait at all */
	CHECK_TRUE( (now + 100*0.8) <= time_now_ms() );
}

TEST(timeout, comparator_test)
{
    /* Comparator margin */
    CHECK_FALSE(timeout_check_elapsed(10, 0, 11));
    CHECK_TRUE (timeout_check_elapsed(10, 0, 10));

    /* Considering timer wrap */
    CHECK_FALSE(timeout_check_elapsed(0xFFFFFFFE, 0xFFFFFFFE, 3));
    CHECK_FALSE(timeout_check_elapsed(0xFFFFFFFF, 0xFFFFFFFE, 3));
    CHECK_FALSE(timeout_check_elapsed(0x00000000, 0xFFFFFFFE, 3));
    CHECK_TRUE (timeout_check_elapsed(0x00000001, 0xFFFFFFFE, 3));
    CHECK_TRUE (timeout_check_elapsed(0x00000002, 0xFFFFFFFE, 3));
    CHECK_TRUE (timeout_check_elapsed(0x00000003, 0xFFFFFFFE, 3));
    CHECK_TRUE (timeout_check_elapsed(0x00000004, 0xFFFFFFFE, 3));
    CHECK_TRUE (timeout_check_elapsed(0x00000005, 0xFFFFFFFE, 3));

    CHECK_FALSE(timeout_check_elapsed(0xFFFFFFFE, 0xFFFFFFFE, 1));
    CHECK_TRUE (timeout_check_elapsed(0xFFFFFFFF, 0xFFFFFFFE, 1));
    CHECK_TRUE (timeout_check_elapsed(0x00000000, 0xFFFFFFFE, 1));
    CHECK_TRUE (timeout_check_elapsed(0x00000001, 0xFFFFFFFE, 1));
    CHECK_TRUE (timeout_check_elapsed(0x00000002, 0xFFFFFFFE, 1));
    CHECK_TRUE (timeout_check_elapsed(0x00000003, 0xFFFFFFFE, 1));
    CHECK_TRUE (timeout_check_elapsed(0x00000004, 0xFFFFFFFE, 1));
    CHECK_TRUE (timeout_check_elapsed(0x00000005, 0xFFFFFFFE, 1));

    CHECK_FALSE(timeout_check_elapsed(0xFFFFFFFE, 0x00000000, 0xFFFFFFFF));
    CHECK_TRUE (timeout_check_elapsed(0xFFFFFFFF, 0x00000000, 0xFFFFFFFF));

    CHECK_FALSE(timeout_check_elapsed(0x0000000E, 0x00000010, 0xFFFFFFFF));
    CHECK_TRUE (timeout_check_elapsed(0x0000000F, 0x00000010, 0xFFFFFFFF));

    /* Tests pass because the subtraction made in check_elapsed
     * removes the wrap effect.
     * It only works if "now" is bigger than "before", considering
     * we are dealing with time, now is bigger than before, always.
     */

    /* Elapsed 0 always return true! */
    CHECK_TRUE (timeout_check_elapsed(0xFFFFFFFD, 0xFFFFFFFE, 0));
    CHECK_TRUE (timeout_check_elapsed(0xFFFFFFFE, 0xFFFFFFFE, 0));

    /* Ok, what if I wanted to test now against a number in the future?
     * Simple! Transform later in before with a stupid big number and
     *  check against it! */
    uint32_t stupid_big_number = 0x80000000; /* ~ 25 days */
    uint32_t later = 0x00000010;
    uint32_t before = later - stupid_big_number;

    CHECK_FALSE(timeout_check_elapsed(0xFFFFFF00, before, stupid_big_number));
    CHECK_FALSE(timeout_check_elapsed(0xFFFFFFFF, before, stupid_big_number));
    CHECK_FALSE(timeout_check_elapsed(0x00000000, before, stupid_big_number));
    CHECK_FALSE(timeout_check_elapsed(0x0000000F, before, stupid_big_number));
    CHECK_TRUE (timeout_check_elapsed(0x00000010, before, stupid_big_number));
    CHECK_TRUE (timeout_check_elapsed(0x00000011, before, stupid_big_number));

    /* As we are good programmers, we wrapped the algorithm in a new function */
    CHECK_FALSE(timeout_check_reached(0x00000010, 0xFFFFFF00));
    CHECK_FALSE(timeout_check_reached(0x00000010, 0xFFFFFFFF));
    CHECK_FALSE(timeout_check_reached(0x00000010, 0x00000000));
    CHECK_FALSE(timeout_check_reached(0x00000010, 0x0000000F));
    CHECK_TRUE (timeout_check_reached(0x00000010, 0x00000010));
    CHECK_TRUE (timeout_check_reached(0x00000010, 0x00000011));

    /* Wrap around whooooooooooooooooooooooooooooooo? */
}
