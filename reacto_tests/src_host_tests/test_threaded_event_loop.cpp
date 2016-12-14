/*
 * This is a test that shows how the Event Loop can be used.
 *
 * It is composed of a "main" thread where main_loop_run is called
 *  repeatedly, and a thread that generates events and pushes it into the
 *  event queue.
 *
 * One `event_loop` is created.
 * One `event_queue` for the data type is created.
 * After, a `slot_eq` is created and initialized to call our handler and then
 * connected to the queue's signal.
 *
 * Now initialization is done and the producer thread is spawned and tests run.
 *
 */

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <pthread.h>
#include <unistd.h>

#include <reusables/linked_list.hpp>

extern "C"
{
    #include <main_loop.h>
    #include <queue.h>
    #include <signal_slot_queue.h>
}

static const int test_duration = 100;

TEST_GROUP(ThreadedEventLoop) {};

/* A container for all the example data */
struct s_cut
{
    slot_queue_t slot;
    queue_t queue;
    main_loop_t loop;
    int buffer[4];
};

/* Helper macro */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/*
 * This is the event handler, it is called every time there's an event
 * to be processed.
 */
static int event_handler (queue_t * queue)
{
    /* Retrieve queue's containing struct */
    struct s_cut * cut = container_of(queue, struct s_cut, queue);

    /* Get the event data from the queue */
    int event_data;
    queue_peek(queue, cut->buffer, &event_data);

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
    main_loop_init(&cut->loop, main_loop_strategy_priority_queue);

    /* Creates a queue and add it to the loop */
    queue_init(&cut->queue, 4);
    main_loop_add_queue(&cut->loop, queue_interface(&cut->queue), 0);

    /* Creates the slot with the handler and connect it to
     * the queue's signal */
    slot_queue_init(&cut->slot, event_handler);
    slot_queue_connect(&cut->slot, queue_signal(&cut->queue));
}

void * thread_handler (void * opaque_cut)
{
    struct s_cut *cut = (struct s_cut *)opaque_cut;
    const int n = test_duration;

    for (int i = 0; i < n; i++)
    {
        if (!queue_full(&cut->queue))
        {
            /* Acquire event data */
            int event_data = i;

            /* Push it to the queue */
            queue_push(&cut->queue, cut->buffer, event_data);
        }
        else
        {
            /* If the queue is full, we try it again later */
            pthread_yield();
            i--;
        }
    }

    main_loop_quit(&cut->loop);
    return NULL;
}

TEST(ThreadedEventLoop, threaded)
{
    static struct s_cut cut;
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
    pthread_create(&thread, NULL, thread_handler, &cut);


    main_loop_run(&cut.loop);
    pthread_join(thread, NULL);

    mock().clear();
}
