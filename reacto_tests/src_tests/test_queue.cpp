#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <string.h>

#include <reacto/reusables/linked_list.hpp>

extern "C"
{
    #include <reacto/queue.h>
    #include <reacto/main_loop.h>
}

struct my_event
{
    int data;
};

struct my_event data_queue[256];

TEST_GROUP(Queue)
{
    queue_t cut;

    void setup ()
    {
        memset((void *)data_queue, 0xFF, sizeof(data_queue));
        int n = queue_init(&cut, 256);
        CHECK_EQUAL((int)(sizeof(data_queue) / sizeof(data_queue[0])), n);
    }

    void teardown ()
    {
        queue_deinit(&cut);
        mock().clear();
    }
};

TEST(Queue, enque)
{
    struct my_event event;
    event.data = 10;

    int result = queue_push (&cut, data_queue, event);
    CHECK_EQUAL(0, result);

    struct my_event poped;
    result = queue_peek (&cut, data_queue, &poped);
    fast_ring_fifo_read_increment(&cut.fifo);
    CHECK_EQUAL(0, result);

    MEMCMP_EQUAL(&event, &poped, sizeof(event));
}


TEST(Queue, enque_full)
{
    struct my_event event;

    int result = 0;
    int i = 0;

    while (result == 0)
    {
        event.data = i++;
        result = queue_push (&cut, data_queue, event);
    }
    CHECK_EQUAL(255, fast_ring_fifo_count(&cut.fifo));

    for (i = 0; i < 255; i++)
        CHECK_EQUAL(i, data_queue[i].data);

    i = 0;
    result = 0;
    while (result == 0)
    {
        struct my_event poped;
        result = queue_peek (&cut, data_queue, &poped);
        fast_ring_fifo_read_increment(&cut.fifo);
        if (result == 0)
            CHECK_EQUAL(i, poped.data);
        i++;
    }
}

TEST(Queue, inval)
{
    queue_t queue;

    mock().strictOrder();

    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: Invalid Pointer")
            .ignoreOtherParameters();
    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: Unable to initialize Fast Ring FIFO, size is too small")
            .ignoreOtherParameters();

    queue_init(0, 0);
    queue_init(&queue, 0);
}

TEST(Queue, remove_on_deinit)
{
    main_loop_t loop;
    queue_t queue;

    queue_init(&queue, 2);
    main_loop_init(&loop, main_loop_strategy_priority_queue);
    main_loop_add_queue(&loop, &queue.itf, 0);
    CHECK_EQUAL(&queue.itf, loop.root);
    CHECK_EQUAL(&loop, queue.itf.loop);
    queue_deinit(&queue);
    CHECK_EQUAL(0, loop.root);
    CHECK_EQUAL(0, queue.itf.loop);
}

TEST(Queue, remove_on_deinit2)
{
    main_loop_t loop;
    queue_t queue;

    queue_init(&queue, 2);
    main_loop_init(&loop, main_loop_strategy_priority_queue);
    main_loop_add_queue(&loop, &queue.itf, 0);
    CHECK_EQUAL(&queue.itf, loop.root);
    CHECK_EQUAL(&loop, queue.itf.loop);
    main_loop_deinit(&loop);
    CHECK_EQUAL(0, loop.root);
    CHECK_EQUAL(0, queue.itf.loop);
}

