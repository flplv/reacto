#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <string.h>
#include <stdint.h>

#include <reacto/reusables/linked_list.hpp>

extern "C"
{
    #include <reacto/main_loop.h>

    /* Faking the time_now */
    static uint32_t injected_now = 0;
    static uint32_t time_now ()
    {
        return injected_now;
    }

    #include <timed_queue.c>
}


TEST_GROUP(TimedQueue)
{
    timed_queue_t cut;

    void setup ()
    {
        injected_now = 0;
        timed_queue_init(&cut);
    }

    void teardown ()
    {
        timed_queue_deinit(&cut);
        mock().clear();
    }
};

TEST(TimedQueue, inval)
{
    mock().strictOrder();

    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: Invalid Pointer")
            .ignoreOtherParameters();

    timed_queue_init(0);
}

void handler (timed_event_t * ev)
{
    CHECK_TRUE(ev);

    mock().actualCall("handler")
            .withIntParameter("timestamp", (int)ev->link_timestamp + (int)ev->timeout);

    CHECK_EQUAL(0, ev->ll.next);
    CHECK_EQUAL(0, ev->ll.prev);
}


TEST(TimedQueue, link_relink_update)
{
    timed_queue_t queue;
    timed_event_t ev1, ev2;

    timed_queue_init(&queue);
    timed_event_init(&ev1, 1000, handler);
    timed_event_init(&ev2, 500, handler);

    timed_queue_link(&queue, &ev1);
    timed_queue_link(&queue, &ev2);

    timed_queue_link_update_timeout(&queue, &ev2, 2000);
    CHECK_EQUAL(&ev1, queue.root);
    CHECK_EQUAL(0, ev1.ll.prev);
    CHECK_EQUAL(0, ev2.ll.next);
    CHECK_EQUAL(&ev2.ll, ev1.ll.next);
    CHECK_EQUAL(&ev1.ll, ev2.ll.prev);
}

TEST(TimedQueue, link_unlink)
{
    timed_queue_t queue;
    timed_event_t ev1, ev2;

    timed_queue_init(&queue);
    timed_event_init(&ev2, 500, handler);
    timed_event_init(&ev1, 1000, handler);
    CHECK_EQUAL(0, queue.root);
    CHECK_EQUAL(0, ev1.ll.prev);
    CHECK_EQUAL(0, ev2.ll.prev);
    CHECK_EQUAL(0, ev1.ll.next);
    CHECK_EQUAL(0, ev2.ll.next);

    timed_queue_link(&queue, &ev1);
    timed_queue_link(&queue, &ev2);
    CHECK_EQUAL(&ev2, queue.root);
    CHECK_EQUAL(0, ev2.ll.prev);
    CHECK_EQUAL(0, ev1.ll.next);
    CHECK_EQUAL(&ev1.ll, ev2.ll.next);
    CHECK_EQUAL(&ev2.ll, ev1.ll.prev);

    timed_queue_unlink(&queue, &ev1);
    timed_queue_unlink(&queue, &ev2);
    CHECK_EQUAL(0, queue.root);
    CHECK_EQUAL(0, ev1.ll.prev);
    CHECK_EQUAL(0, ev2.ll.prev);
    CHECK_EQUAL(0, ev1.ll.next);
    CHECK_EQUAL(0, ev2.ll.next);
}

TEST(TimedQueue, link_unlink_on_deinit)
{
    timed_queue_t queue;
    timed_event_t ev1, ev2;

    timed_queue_init(&queue);
    timed_event_init(&ev2, 500, handler);
    timed_event_init(&ev1, 1000, handler);
    CHECK_EQUAL(0, queue.root);
    CHECK_EQUAL(0, ev1.ll.prev);
    CHECK_EQUAL(0, ev2.ll.prev);
    CHECK_EQUAL(0, ev1.ll.next);
    CHECK_EQUAL(0, ev2.ll.next);

    timed_queue_link(&queue, &ev1);
    timed_queue_link(&queue, &ev2);
    CHECK_EQUAL(&ev2, queue.root);
    CHECK_EQUAL(0, ev2.ll.prev);
    CHECK_EQUAL(0, ev1.ll.next);
    CHECK_EQUAL(&ev1.ll, ev2.ll.next);
    CHECK_EQUAL(&ev2.ll, ev1.ll.prev);

    timed_queue_deinit(&queue);
    CHECK_EQUAL(0, queue.root);
    CHECK_EQUAL(0, ev1.ll.prev);
    CHECK_EQUAL(0, ev2.ll.prev);
    CHECK_EQUAL(0, ev1.ll.next);
    CHECK_EQUAL(0, ev2.ll.next);
}

TEST(TimedQueue, link_order_2)
{
    timed_event_t evs[5];
    main_loop_t loop;

    injected_now = 0;

    for (uint32_t i = 0; i < 5; i++) {
        timed_event_init(&evs[i], (i+1) * 100, handler);
        timed_queue_link(&cut, &evs[i]);
    }

    CHECK_EQUAL(5, cut.cnt_cache);

    main_loop_init(&loop, main_loop_strategy_fare);
    loop.looping = false;
    main_loop_add_queue(&loop, timed_queue_interface(&cut), 0);

    injected_now = 0;
    main_loop_run(&loop);
    mock().checkExpectations();

    injected_now = 99;
    main_loop_run(&loop);
    mock().checkExpectations();

    injected_now = 100;
    mock().expectOneCall("handler")
            .withParameter("timestamp", 100);
    main_loop_run(&loop);
    mock().checkExpectations();
    CHECK_EQUAL(4, cut.cnt_cache);

    injected_now = 199;
    main_loop_run(&loop);
    mock().checkExpectations();

    injected_now = 299;
    mock().expectOneCall("handler")
            .withParameter("timestamp", 200);
    main_loop_run(&loop);
    mock().checkExpectations();
    CHECK_EQUAL(3, cut.cnt_cache);


    injected_now = 300;
    mock().expectOneCall("handler")
            .withParameter("timestamp", 300);
    main_loop_run(&loop);
    mock().checkExpectations();
    CHECK_EQUAL(2, cut.cnt_cache);


    injected_now = 500;
    mock().expectOneCall("handler")
            .withParameter("timestamp", 400);
    mock().expectOneCall("handler")
            .withParameter("timestamp", 500);
    main_loop_run(&loop);
    mock().checkExpectations();
    CHECK_EQUAL(0, cut.cnt_cache);

    timed_queue_deinit(&cut);
}


TEST(TimedQueue, link_order)
{
    timed_event_t evs[100];
    main_loop_t loop;

    for (uint32_t i = 0; i < 100; i++) {
        timed_event_init(&evs[i], 1000000 - (i * 100), handler);
        timed_queue_link(&cut, &evs[i]);
    }

    CHECK_EQUAL(100, cut.cnt_cache);

    mock().strictOrder();

    for (int i = 99; i >= 0; i--) {
        mock().expectOneCall("handler")
                .withParameter("timestamp", 1000000 - (i * 100));
    }

    main_loop_init(&loop, main_loop_strategy_fare);
    loop.looping = false;
    main_loop_add_queue(&loop, timed_queue_interface(&cut), 0);

    injected_now = 10000000;
    main_loop_run(&loop);
    timed_queue_deinit(&cut);
    mock().checkExpectations();
}

TEST(TimedQueue, remove_on_deinit)
{
    main_loop_t loop;
    timed_queue_t queue;

    timed_queue_init(&queue);
    main_loop_init(&loop, main_loop_strategy_priority_queue);
    main_loop_add_queue(&loop, &queue.itf, 0);
    CHECK_EQUAL(&queue.itf, loop.root);
    CHECK_EQUAL(&loop, queue.itf.loop);
    timed_queue_deinit(&queue);
    CHECK_EQUAL(0, loop.root);
    CHECK_EQUAL(0, queue.itf.loop);
}

TEST(TimedQueue, remove_on_deinit2)
{
    main_loop_t loop;
    timed_queue_t queue;

    timed_queue_init(&queue);
    main_loop_init(&loop, main_loop_strategy_priority_queue);
    main_loop_add_queue(&loop, &queue.itf, 0);
    CHECK_EQUAL(&queue.itf, loop.root);
    CHECK_EQUAL(&loop, queue.itf.loop);
    main_loop_deinit(&loop);
    CHECK_EQUAL(0, loop.root);
    CHECK_EQUAL(0, queue.itf.loop);
}
