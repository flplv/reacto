#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <string.h>

#include <reacto/reusables/linked_list.hpp>

extern "C"
{
    #define REACTO_N_OF_QUEUES 5
    #define REACTO_MAX_N_OF_HANDLERS_PER_QUEUE 5
    #include <reacto/context_factory.template>
    #include <reacto/reusables/fast_ring_fifo.h>
}


static struct reacto_queues_context queues_context;
static struct reacto_timed_queue_context timed_queue_context;

static int h (queue_t * q)
{
    mock().actualCall("h").withPointerParameter("q", q);
    return 0;
}

TEST_GROUP(Context)
{
    void setup ()
    {
        typeof(queues_context.queue_buffer_sizes) sizes = {
                2, 4, 8, 16, 32
        };

        typeof(queues_context.queue_handlers) handlers = {
                { h, h, h, h, h },
                { h, h, h, h, h },
                { h, h, h, h, h },
                { h, h, h, h, h },
                { h, h, h, h, h },
        };

        memcpy(queues_context.queue_buffer_sizes, sizes, sizeof(sizes));
        memcpy(queues_context.queue_handlers, handlers, sizeof(handlers));
        timed_queue_context.high_priority = false;

        memset(&reacto_context_private_data, 0x00, sizeof(reacto_context_private_data));
    }

    void teardown ()
    {
        mock().clear();
    }
};

TEST(Context, handlers)
{
    int r = reacto_context_factory(main_loop_strategy_fare,
                                   &timed_queue_context,
                                   &queues_context);

    CHECK_EQUAL(0 , r);

    mock().expectNCalls(5, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[0]);
    mock().expectNCalls(5, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[1]);
    mock().expectNCalls(5, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[2]);
    mock().expectNCalls(5, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[3]);
    mock().expectNCalls(5, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[4]);

    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[0].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[1].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[2].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[3].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[4].fifo);

    auto loop = reacto_context_main_loop();
    loop->looping = false;
    main_loop_run(loop);
}

TEST(Context, incomplete_handlers)
{
    typeof(queues_context.queue_handlers) handlers = {
            { h, h, 0, 0, 0 },
            { h, h, 0, 0, 0 },
            { h, h, 0, 0, 0 },
            { h, h, 0, 0, 0 },
            { h, h, 0, 0, 0 },
    };

    memcpy(queues_context.queue_handlers, handlers, sizeof(handlers));

    int r = reacto_context_factory(main_loop_strategy_fare,
                                   &timed_queue_context,
                                   &queues_context);

    CHECK_EQUAL(0 , r);

    mock().expectNCalls(2, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[0]);
    mock().expectNCalls(2, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[1]);
    mock().expectNCalls(2, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[2]);
    mock().expectNCalls(2, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[3]);
    mock().expectNCalls(2, "h")
            .withPointerParameter("q", &reacto_context_private_data.queues[4]);

    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[0].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[1].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[2].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[3].fifo);
    fast_ring_fifo_write_increment(&reacto_context_private_data.queues[4].fifo);

    auto loop = reacto_context_main_loop();
    loop->looping = false;
    main_loop_run(loop);
}

TEST(Context, timed_prio)
{
    timed_queue_context.high_priority = true;

    int r = reacto_context_factory(main_loop_strategy_fare,
                                   &timed_queue_context,
                                   &queues_context);
    auto loop = reacto_context_main_loop();

    CHECK_EQUAL(0 , r);

    CHECK_EQUAL(&reacto_context_private_data.timed_queue.itf, loop->root);
    CHECK_EQUAL(&reacto_context_private_data.queues[0].itf, linked_list_next(loop->root, ll));
    CHECK_EQUAL(&reacto_context_private_data.queues[4].itf, linked_list_last(loop->root, ll));
}

TEST(Context, timed_not_prio)
{
    timed_queue_context.high_priority = false;

    int r = reacto_context_factory(main_loop_strategy_fare,
                                   &timed_queue_context,
                                   &queues_context);
    auto loop = reacto_context_main_loop();

    CHECK_EQUAL(0 , r);

    CHECK_EQUAL(&reacto_context_private_data.queues[0].itf, loop->root);
    CHECK_EQUAL(&reacto_context_private_data.queues[1].itf, linked_list_next(loop->root, ll));
    CHECK_EQUAL(&reacto_context_private_data.timed_queue.itf, linked_list_last(loop->root, ll));
}

TEST(Context, fifos_order)
{
    int r = reacto_context_factory(main_loop_strategy_fare,
                                   &timed_queue_context,
                                   &queues_context);

    CHECK_EQUAL(0 , r);
    CHECK_EQUAL(2 , fast_ring_fifo_num_of_slots(&reacto_context_private_data.queues[0].fifo));
    CHECK_EQUAL(4 , fast_ring_fifo_num_of_slots(&reacto_context_private_data.queues[1].fifo));
    CHECK_EQUAL(8 , fast_ring_fifo_num_of_slots(&reacto_context_private_data.queues[2].fifo));
    CHECK_EQUAL(16, fast_ring_fifo_num_of_slots(&reacto_context_private_data.queues[3].fifo));
    CHECK_EQUAL(32, fast_ring_fifo_num_of_slots(&reacto_context_private_data.queues[4].fifo));
}

TEST(Context, bad_queue)
{
    mock().expectOneCall("_log_file_line").ignoreOtherParameters();

    queues_context.queue_buffer_sizes[0] = 0;

    int r = reacto_context_factory(main_loop_strategy_fare,
                                   &timed_queue_context,
                                   &queues_context);

    CHECK_EQUAL(-1 , r);
}

TEST(Context, getters)
{
    CHECK_EQUAL(0, reacto_context_main_loop());
    CHECK_EQUAL(0, reacto_context_timed_queue());
    CHECK_EQUAL(0, reacto_context_queue(0));
    CHECK_EQUAL(0, reacto_context_queue(1));
    CHECK_EQUAL(0, reacto_context_queue(2));
    CHECK_EQUAL(0, reacto_context_queue(3));
    CHECK_EQUAL(0, reacto_context_queue(4));
    CHECK_EQUAL(0, reacto_context_queue(127981));

    int r = reacto_context_factory(main_loop_strategy_fare,
                                   &timed_queue_context,
                                   &queues_context);
    CHECK_EQUAL(0 , r);

    CHECK_EQUAL(&reacto_context_private_data.loop, reacto_context_main_loop());
    CHECK_EQUAL(&reacto_context_private_data.timed_queue, reacto_context_timed_queue());
    CHECK_EQUAL(&reacto_context_private_data.queues[0], reacto_context_queue(0));
    CHECK_EQUAL(&reacto_context_private_data.queues[1], reacto_context_queue(1));
    CHECK_EQUAL(&reacto_context_private_data.queues[2], reacto_context_queue(2));
    CHECK_EQUAL(&reacto_context_private_data.queues[3], reacto_context_queue(3));
    CHECK_EQUAL(&reacto_context_private_data.queues[4], reacto_context_queue(4));
}
