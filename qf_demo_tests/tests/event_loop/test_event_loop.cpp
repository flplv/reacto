#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*
 * Basic testing guideline:
 * 1) Test all functionalities
 * 2) Test buffer boundaries
 * 3) Test invalid arguments
 */
#include "../reusables/linked_list.hpp"

extern "C"
{
    #include <event_loop/event_loop.h>
    #include <event_loop/event_queue.h>
    #include <event_loop/signal_slot_eq.h>
}

TEST_GROUP(EventLoop)
{
    event_loop_t cut;

    void setup ()
    {
    }

    void teardown ()
    {
        mock().clear();
    }
};

TEST(EventLoop, queue_priority)
{
    event_queue_t queue0;
    event_queue_t queue1;
    event_queue_t queue2;
    event_queue_t queue3;
    event_queue_t queue4;
    event_queue_t queue5;

    event_loop_init(&cut, event_loop_strategy_consume_all_at_once);
    event_queue_init(&queue0, 2);
    event_queue_init(&queue1, 2);
    event_queue_init(&queue2, 2);
    event_queue_init(&queue3, 2);
    event_queue_init(&queue4, 2);
    event_queue_init(&queue5, 2);

    event_loop_add_queue(&cut, &queue4, 5);
    event_loop_add_queue(&cut, &queue0, 0);
    event_loop_add_queue(&cut, &queue1, 1);
    event_loop_add_queue(&cut, &queue3, 2);
    event_loop_add_queue(&cut, &queue2, 2);
    event_loop_add_queue(&cut, &queue5, 121);

    CHECK_EQUAL(&queue0, cut.root);
    CHECK_EQUAL(&queue1, linked_list_next(cut.root, ll));
    CHECK_EQUAL(&queue2, linked_list_next(&queue1, ll));
    CHECK_EQUAL(&queue3, linked_list_next(&queue2, ll));
    CHECK_EQUAL(&queue4, linked_list_next(&queue3, ll));
    CHECK_EQUAL(&queue5, linked_list_last(cut.root, ll));

    event_loop_deinit(&cut);
}

TEST(EventLoop, queue_deinit)
{
    event_queue_t queue1;
    event_queue_t queue2;

    event_loop_init(&cut, event_loop_strategy_consume_all_at_once);
    event_queue_init(&queue1, 2);
    event_queue_init(&queue2, 2);

    event_loop_add_queue(&cut, &queue1, 0);
    event_loop_add_queue(&cut, &queue2, 0);

    event_loop_deinit(&cut);
    CHECK_EQUAL(1, linked_list_count(&queue1, ll))
    CHECK_EQUAL(1, linked_list_count(&queue2, ll))
    CHECK_EQUAL(0, queue1.loop);
    CHECK_EQUAL(0, queue2.loop);
}

static int event_buf[4];

int slot_handler (event_queue_t * queue)
{
    int d;
    event_queue_peek(queue, event_buf, &d);

    mock().actualCall("slot_handler")
        .withPointerParameter("queue", queue)
        .withIntParameter("event", d);

    return mock().returnValue().getIntValue();
}

TEST(EventLoop, process_events)
{
    slot_eq_t slot;
    event_queue_t queue;
    event_loop_t loop;

    slot_eq_init(&slot, slot_handler);
    event_queue_init(&queue, 4);
    event_loop_init(&loop, event_loop_strategy_consume_all_at_once);

    event_loop_add_queue(&loop, &queue, 0);
    slot_eq_connect(&slot, event_queue_signal(&queue));

    event_queue_push(&queue, event_buf, 10);
    event_queue_push(&queue, event_buf, 24);
    event_queue_push(&queue, event_buf, 32);

    mock().strictOrder();

    mock().expectOneCall("slot_handler")
            .withParameter("queue", &queue)
            .withParameter("event", 10)
            .andReturnValue(0);

    mock().expectOneCall("slot_handler")
            .withParameter("queue", &queue)
            .withParameter("event", 24)
            .andReturnValue(0);

    mock().expectOneCall("slot_handler")
            .withParameter("queue", &queue)
            .withParameter("event", 32)
            .andReturnValue(0);

    event_loop_process(&loop);
}

static int event_buf1[4];
static int event_buf2[4];

int slot_handler_1 (event_queue_t * queue)
{
    int d;
    event_queue_peek(queue, event_buf1, &d);

    mock().actualCall("slot_handler_1")
        .withPointerParameter("queue", queue)
        .withIntParameter("event", d);

    return mock().returnValue().getIntValue();
}

int slot_handler_2 (event_queue_t * queue)
{
    int d;
    event_queue_peek(queue, event_buf2, &d);

    mock().actualCall("slot_handler_2")
        .withPointerParameter("queue", queue)
        .withIntParameter("event", d);

    return mock().returnValue().getIntValue();
}

TEST(EventLoop, process_order)
{
    slot_eq_t slot1, slot2;
    event_queue_t queue1, queue2;
    event_loop_t loop;

    slot_eq_init(&slot1, slot_handler_1);
    slot_eq_init(&slot2, slot_handler_2);
    event_queue_init(&queue1, 4);
    event_queue_init(&queue2, 4);
    event_loop_init(&loop, event_loop_strategy_consume_all_at_once);

    event_loop_add_queue(&loop, &queue1, 0);
    event_loop_add_queue(&loop, &queue2, 1);
    slot_eq_connect(&slot1, event_queue_signal(&queue1));
    slot_eq_connect(&slot2, event_queue_signal(&queue2));

    event_queue_push(&queue1, event_buf1, 10);
    event_queue_push(&queue2, event_buf2, 24);
    event_queue_push(&queue1, event_buf1, 32);
    event_queue_push(&queue2, event_buf2, 64);

    mock().strictOrder();

    mock().expectOneCall("slot_handler_1")
            .withParameter("queue", &queue1)
            .withParameter("event", 10)
            .andReturnValue(0);

    mock().expectOneCall("slot_handler_1")
            .withParameter("queue", &queue1)
            .withParameter("event", 32)
            .andReturnValue(0);

    mock().expectOneCall("slot_handler_2")
            .withParameter("queue", &queue2)
            .withParameter("event", 24)
            .andReturnValue(0);

    mock().expectOneCall("slot_handler_2")
            .withParameter("queue", &queue2)
            .withParameter("event", 64)
            .andReturnValue(0);

    event_loop_process(&loop);
}
