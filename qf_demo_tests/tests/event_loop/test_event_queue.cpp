#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <string.h>

#include <reusables/linked_list.hpp>

extern "C"
{
    #include <event_loop/event_queue.h>
    #include <event_loop/event_loop.h>
}

struct my_event
{
    int data;
};

struct my_event data_queue[256];

TEST_GROUP(EventQueue)
{
    event_queue_t cut;

    void setup ()
    {
        memset((void *)data_queue, 0xFF, sizeof(data_queue));
        int n = event_queue_init(&cut, 256);
        CHECK_EQUAL((int)(sizeof(data_queue) / sizeof(data_queue[0])), n);
    }

    void teardown ()
    {
        event_queue_deinit(&cut);
        mock().clear();
    }
};

TEST(EventQueue, enque)
{
    struct my_event event;
    event.data = 10;

    int result = event_queue_push (&cut, data_queue, event);
    CHECK_EQUAL(0, result);

    struct my_event poped;
    result = event_queue_peek (&cut, data_queue, &poped);
    fast_ring_fifo_read_increment(&cut.fifo);
    CHECK_EQUAL(0, result);

    MEMCMP_EQUAL(&event, &poped, sizeof(event));
}


TEST(EventQueue, enque_full)
{
    struct my_event event;

    int result = 0;
    int i = 0;

    while (result == 0)
    {
        event.data = i++;
        result = event_queue_push (&cut, data_queue, event);
    }
    CHECK_EQUAL(255, fast_ring_fifo_count(&cut.fifo));

    for (i = 0; i < 255; i++)
        CHECK_EQUAL(i, data_queue[i].data);

    i = 0;
    result = 0;
    while (result == 0)
    {
        struct my_event poped;
        result = event_queue_peek (&cut, data_queue, &poped);
        fast_ring_fifo_read_increment(&cut.fifo);
        if (result == 0)
            CHECK_EQUAL(i, poped.data);
        i++;
    }
}

TEST(EventQueue, inval)
{
    event_queue_t queue;

    mock().strictOrder();

    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: Invalid Pointer")
            .ignoreOtherParameters();
    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: Unable to initialize Fast Ring FIFO, size is too small")
            .ignoreOtherParameters();

    event_queue_init(0, 0);
    event_queue_init(&queue, 0);
}

TEST(EventQueue, remove_on_deinit)
{
    event_loop_t loop;
    event_queue_t queue;

    event_queue_init(&queue, 2);
    event_loop_init(&loop, event_loop_strategy_consume_all_at_once);
    event_loop_add_queue(&loop, &queue, 0);
    CHECK_EQUAL(&queue, loop.root);
    CHECK_EQUAL(&loop, queue.loop);
    event_queue_deinit(&queue);
    CHECK_EQUAL(0, loop.root);
    CHECK_EQUAL(0, queue.loop);
}

TEST(EventQueue, remove_on_deinit2)
{
    event_loop_t loop;
    event_queue_t queue;

    event_queue_init(&queue, 2);
    event_loop_init(&loop, event_loop_strategy_consume_all_at_once);
    event_loop_add_queue(&loop, &queue, 0);
    CHECK_EQUAL(&queue, loop.root);
    CHECK_EQUAL(&loop, queue.loop);
    event_loop_deinit(&loop);
    CHECK_EQUAL(0, loop.root);
    CHECK_EQUAL(0, queue.loop);
}

