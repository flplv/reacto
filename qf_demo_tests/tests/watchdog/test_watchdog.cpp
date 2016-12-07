#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "../reusables/linked_list.hpp"

extern "C"
{
    #include <watchdog/watchdog.c>
}

TEST_GROUP(Watchdog)
{
    void setup ()
    {
        root = (watchdog_t *)0;
    }

    void teardown ()
    {
        mock().clear();
    }
};

TEST(Watchdog, hw_init_deinit)
{
    watchdog_t cut, cut2;
    mock().expectOneCall("hardware_watchdog_init");
    int r = watchdog_init(&cut, 1, "cut");
    CHECK_EQUAL (0, r);
    mock().checkExpectations();

    r = watchdog_init(&cut2, 1, "cut");
    CHECK_EQUAL (0, r);

    watchdog_deinit(&cut);

    mock().expectOneCall("hardware_watchdog_deinit");
    watchdog_deinit(&cut2);
    mock().checkExpectations();
}

TEST(Watchdog, init_inval_arg)
{
    mock().expectOneCall("_log_impl_file_line")
            .withParameter("msg", "Error: Invalid Pointer")
            .ignoreOtherParameters();
    int r = watchdog_init(NULL, 0, "cut");
    CHECK_EQUAL (-1, r);
}

TEST(Watchdog, list_size)
{
    mock().expectOneCall("hardware_watchdog_init");
    mock().expectOneCall("hardware_watchdog_deinit");

    CHECK_EQUAL(0, linked_list_count(root, list_head));

    watchdog_t cut, cut2;
    watchdog_init(&cut, 1, "cut");
    CHECK_EQUAL(1, linked_list_count(root, list_head));

    watchdog_init(&cut2, 1, "cut");
    CHECK_EQUAL(2, linked_list_count(root, list_head));

    watchdog_deinit(&cut);
    CHECK_EQUAL(1, linked_list_count(root, list_head));

    watchdog_init(&cut, 1, "cut");
    CHECK_EQUAL(2, linked_list_count(root, list_head));

    watchdog_deinit(&cut);
    CHECK_EQUAL(1, linked_list_count(root, list_head));

    watchdog_deinit(&cut2);
    CHECK_EQUAL(0, linked_list_count(root, list_head));
}

TEST(Watchdog, list_root)
{
    mock().expectOneCall("hardware_watchdog_init");
    mock().expectOneCall("hardware_watchdog_deinit");

    watchdog_t cut, cut2;
    watchdog_init(&cut, 1, "cut");
    CHECK_EQUAL(&cut, root);

    watchdog_init(&cut2, 1, "cut");

    watchdog_deinit(&cut);
    CHECK_EQUAL(&cut2, root)

    watchdog_init(&cut, 1, "cut");

    watchdog_deinit(&cut2);
    CHECK_EQUAL(&cut, root)

    watchdog_deinit(&cut);
    CHECK_EQUAL(0, root)
}

TEST(Watchdog, expire)
{
    mock().expectOneCall("hardware_watchdog_init");
    mock().expectOneCall("hardware_watchdog_deinit");

    mock().expectNCalls(4, "hardware_watchdog_kick");

    mock().expectOneCall("_log_impl")
            .withParameter("msg", "cut");
    mock().expectOneCall("_log_impl")
            .withParameter("msg", "(Watchdog expired)");

    watchdog_t cut, cut2;
    timeout_t tout;

    watchdog_init(&cut, 10, "cut");
    watchdog_init(&cut2, 100, "cut2");

    watchdog_enter(&cut);
    watchdog_enter(&cut2);
    timeout_init(&tout); /* Tout init, this is the base time */

    timeout_sleep(&tout, 2); /* Wait 2ms after tout init */
    watchdog_periodic(); /* 1st kick */

    timeout_sleep(&tout, 4); /* Wait 4ms after tout init */
    watchdog_periodic(); /* 2nd kick */

    timeout_sleep(&tout, 6); /* Wait 6ms after tout init */
    watchdog_periodic(); /* 3rd kick */

    timeout_sleep(&tout, 8); /* Wait 8ms after tout init */
    watchdog_periodic(); /* 4rd kick */

    timeout_sleep(&tout, 11); /* Wait 11ms after tout init */
    watchdog_periodic(); /* No kick expected here */

    watchdog_deinit(&cut);
    watchdog_deinit(&cut2);
}


TEST(Watchdog, enter_exit)
{
    mock().expectOneCall("hardware_watchdog_init");
    mock().expectOneCall("hardware_watchdog_deinit");

    mock().expectNCalls(5, "hardware_watchdog_kick");

    watchdog_t cut, cut2;
    timeout_t tout;

    watchdog_init(&cut, 10, "cut");
    watchdog_init(&cut2, 100, "cut2");

    watchdog_enter(&cut);
    watchdog_enter(&cut2);
    timeout_init(&tout); /* Tout init, this is the base time */

    timeout_sleep(&tout, 2); /* Wait 2ms after tout init */
    watchdog_periodic(); /* 1st kick */

    timeout_sleep(&tout, 4); /* Wait 4ms after tout init */
    watchdog_periodic(); /* 2nd kick */

    timeout_sleep(&tout, 6); /* Wait 6ms after tout init */
    watchdog_periodic(); /* 3rd kick */

    timeout_sleep(&tout, 8); /* Wait 8ms after tout init */
    watchdog_periodic(); /* 4th kick */

    watchdog_exit(&cut); /* Cut exited before expiring */

    timeout_sleep(&tout, 11); /* Wait 11ms after tout init */
    watchdog_periodic(); /* 5th kick */

    watchdog_deinit(&cut);
    watchdog_deinit(&cut2);
}
