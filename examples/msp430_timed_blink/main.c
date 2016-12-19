/*
 * This is a blink example showing deferred jobs.
 *
 * A single push will toggle led0.
 * A double push will toggle led1.
 *
 */


#include <msp430.h>
#include <legacymsp430.h>
#include <stdint.h>
#include <reacto/main_loop.h>
#include <reacto/queue.h>
#include <reacto/timed_queue.h>

#include "../msp430_board.h"


typedef enum button
{
    button_invalid,
    button_0
} button_event_t;

struct button_stream
{
    slot_queue_t slot;
    queue_t queue;
    button_event_t buffer [8];
};

struct time_stream
{
    timed_queue_t queue;
    timed_event_t event;
};

typedef enum led
{
    led_invalid,
    led_0,
    led_1,
} led_event_t;

struct led_stream
{
    slot_queue_t slot;
    queue_t queue;
    led_event_t buffer [8];
};

static main_loop_t loop;
static struct button_stream button_stream;
static struct led_stream led_stream;
static struct time_stream time_stream;
static uint8_t button_accumulator;

static int button_stream_handler (queue_t * queue);
static void delayed_handler (timed_event_t * ev);
static int led_stream_handler (queue_t * queue);
static void single_pressed();
static void double_pressed();

static void button_stream_init()
{
    queue_init(&button_stream.queue, 8);
    slot_queue_init(&button_stream.slot, button_stream_handler);
    slot_queue_connect(&button_stream.slot, queue_signal(&button_stream.queue));
    main_loop_add_queue(&loop, queue_interface(&button_stream.queue), 0);
}

static void led_stream_init()
{
    queue_init(&led_stream.queue, 8);
    slot_queue_init(&led_stream.slot, led_stream_handler);
    slot_queue_connect(&led_stream.slot, queue_signal(&led_stream.queue));
    main_loop_add_queue(&loop, queue_interface(&led_stream.queue), 0);
}

static void time_stream_init()
{
    timed_queue_init(&time_stream.queue);
    main_loop_add_queue(&loop, timed_queue_interface(&time_stream.queue), 0);
    timed_event_init(&time_stream.event, 250, delayed_handler);
}

int main (void)
{
    board_init();

    main_loop_init (&loop, main_loop_strategy_fare);
    button_stream_init();
    led_stream_init();
    time_stream_init();
    button_accumulator = 0;

    __enable_interrupt(); // enable all interrupts

    main_loop_run (&loop);
    /* this will not return until main_loop_quit() is called */
    P1OUT = 0;
    return 0;
}

/*
 * Now the handlers in order of data flow
 */

/* First the PUSH BUTTON Interrupt Handler */
interrupt(PORT1_VECTOR) on_button_pressed(void)
{
    /* Interrupt stuff */
    P1IFG &= ~BUTTON; // P1.3 IFG cleared
    P1IES ^= BUTTON; // toggle the interrupt edge,
    if (P1IN & BUTTON) return; // Ignore one edge

    /* Push event to the queue */
    queue_push (&button_stream.queue, button_stream.buffer, button_0);
}

/* Second the button stream handler */
static int button_stream_handler (queue_t * queue)
{
    button_event_t event = button_invalid;
    queue_peek (&button_stream.queue, button_stream.buffer, &event);

    if (button_accumulator == 0)
    {
        /* First press, increment the accumulator and schedule timeout on */
        button_accumulator += 1;
        timed_queue_link(&time_stream.queue, &time_stream.event);
    }
    else
    {
        /* It is a second press, unschedule the timeout and deal with double press
         * here */
        button_accumulator = 0;
        timed_queue_unlink(&time_stream.queue, &time_stream.event);
        double_pressed(event);
    }

    return 0;
}

/* Third the timed event handler */
static void delayed_handler (timed_event_t * ev)
{
    /* Handler got called, we have a timeout.
     * it only happens in single press */
    button_accumulator = 0;
    single_pressed();
}

static void single_pressed()
{
    /* Push to the queue a led_1 event */
    queue_push (&led_stream.queue, led_stream.buffer, led_0);
}

static void double_pressed()
{
    /* Push to the queue a led_1 event */
    queue_push (&led_stream.queue, led_stream.buffer, led_1);
}

/* 4th the led stream handler */
static int led_stream_handler (queue_t * queue)
{
    led_event_t event = led_invalid;
    queue_peek (&led_stream.queue, led_stream.buffer, &event);

    if (event == led_0)
        P1OUT ^= (LED0); // P1.0 = toggle

    else if (event == led_1)
        P1OUT ^= (LED1); // P1.0 = toggle

    return 0;
}
