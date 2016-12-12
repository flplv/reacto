#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <pthread.h>
#include <unistd.h>

#include "../../tests/reusables/linked_list.hpp"

extern "C"
{
    #include <event_loop/event_loop.h>
    #include <event_loop/event_queue.h>
    #include <event_loop/signal_slot_eq.h>
}

static const int test_duration = 100;

TEST_GROUP(ThreadedEventLoop) {};

/* A container for all the example data */
struct s_cut
{
    slot_eq_t slot;
    event_queue_t queue;
    event_loop_t loop;
    int buffer[4];
    bool thread_running;
};

/* Helper macro */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/*
 * This is the event handler, it is called every time there's an event
 * to be processed.
 */
static int event_handler (event_queue_t * queue)
{
    /* Retrieve queue's containing struct */
    struct s_cut * cut = container_of(queue, struct s_cut, queue);

    /* Get the event data from the queue */
    int event_data;
    event_queue_peek(queue, cut->buffer, &event_data);

    /* Consume event */
    mock().actualCall("event_handler")
        .withIntParameter("event_data", event_data);

    /* Let the caller knows it should keep calling connected handlers */
    return 0;
}

/* Initialize all objects of the example */
static void cut_init (struct s_cut *cut)
{
    /* Creates the loop object */
    event_loop_init(&cut->loop, event_loop_strategy_consume_all_at_once);

    /* Creates a queue and add it to the loop */
    event_queue_init(&cut->queue, 4);
    event_loop_add_queue(&cut->loop, &cut->queue, 0);

    /* Creates the slot with the handler and connect it to
     * the queue's signal */
    slot_eq_init(&cut->slot, event_handler);
    slot_eq_connect(&cut->slot, event_queue_signal(&cut->queue));
}

void * thread_handler (void * opaque_cut)
{
    struct s_cut *cut = (struct s_cut *)opaque_cut;
    const int n = test_duration;

    for (int i = 0; i < n; i++)
    {
        if (!event_queue_full(&cut->queue))
        {
            /* Acquire event data */
            int event_data = i;

            /* Push it to the queue */
            event_queue_push(&cut->queue, cut->buffer, event_data);
        }
        else
        {
            /* If the queue is full, we try it again later */
            pthread_yield();
            i--;
        }
    }

    cut->thread_running = false;
    return NULL;
}

TEST(ThreadedEventLoop, threaded)
{
    struct s_cut cut;
    pthread_t thread;

    mock().strictOrder();

    /* We add a mock expectation before, since CppUTest's mock is
     * not thread safe. */
    for (int i = 0; i < test_duration; i++)
        mock().expectOneCall("event_handler")
              .withParameter("event_data", i);


    /* First we initialize our CUT */
    cut_init(&cut);

    /* Second, we spawn a thread to generate events.
     * Only one thread per queue allowed, the queue is single consumer
     * single producer safe. */
    cut.thread_running = true;
    pthread_create(&thread, NULL, thread_handler, &cut);


    while (cut.thread_running)
    {
        event_loop_process(&cut.loop);
        pthread_yield();
    }

    event_loop_process(&cut.loop);
    pthread_join(thread, NULL);

    mock().clear();
}
