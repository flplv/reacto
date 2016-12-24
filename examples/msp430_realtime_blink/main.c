/*
 * This is a blink example where the events
 * are expected to be processed within 10ms.
 * If it takes more than 10ms, the led1 toggles.
 * Every time you push the button, led0 toggles.
 */

#include <msp430.h>
#include <legacymsp430.h>
#include <reacto/main_loop.h>
#include <reacto/queue.h>
#include <reacto/realtime/ab_timing.h>

#include "../msp430_board.h"

enum button
{
    button_invalid,
    button_0
};

typedef struct
{
    enum button b;
    ab_timing_t ab;
} button_event_t;

enum led
{
    led_invalid,
    led_0,
    led_1
};

typedef struct
{
    enum led l;
    ab_timing_t ab;
} led_event_t;

static slot_queue_t button_slot;
static queue_t button_queue;
static button_event_t button_buffer [8];

static slot_queue_t led_slot;
static queue_t led_queue;
static led_event_t led_buffer [8];

static main_loop_t loop;

static int button_stream_handler (queue_t * queue);
static int led_stream_handler (queue_t * queue);
void time_sleep (uint32_t delay);

static void button_stream_init()
{
    queue_init(&button_queue, 8);
    slot_queue_init(&button_slot, button_stream_handler);
    slot_queue_connect(&button_slot, queue_signal(&button_queue));
    main_loop_add_queue(&loop, queue_interface(&button_queue), 0);
}

static void led_stream_init()
{
    queue_init(&led_queue, 8);
    slot_queue_init(&led_slot, led_stream_handler);
    slot_queue_connect(&led_slot, queue_signal(&led_queue));
    main_loop_add_queue(&loop, queue_interface(&led_queue), 0);
}


int main (void)
{
    board_init();

    main_loop_init (&loop, main_loop_strategy_fare);
    button_stream_init();
    led_stream_init();

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
    button_event_t ev;

    /* Interrupt stuff */
    P1IFG &= ~BUTTON; // P1.3 IFG cleared
    P1IES ^= BUTTON; // toggle the interrupt edge,
    if (P1IN & BUTTON) return; // Ignore one edge

    ev.b = button_0;
    /* New event arrived, we will start the realtime monitor for this event */
    ab_timing_start(&ev.ab);

    /* Push event to the queue */
    queue_push (&button_queue, button_buffer, ev);
}

/* Second the button stream handler */
static int button_stream_handler (queue_t * queue)
{
    static int i = 0;
    button_event_t ev_in;
    led_event_t ev_out;
    queue_peek (&button_queue, button_buffer, &ev_in);

    /* Lets simulate random latency */
    time_sleep(time_now() % 10);

    if (ev_in.b == button_0)
    {
        ev_out.l = led_0;
        /* We copy the ab_timing object to the new event */
        ab_timing_copy(&ev_out.ab, &ev_in.ab);
        queue_push (&led_queue, led_buffer, ev_out);
    }
    return 0;
}

/* Third the led stream handler */
static int led_stream_handler (queue_t * queue)
{
    led_event_t event;
    queue_peek (&led_queue, led_buffer, &event);

    if (event.l == led_0)
        P1OUT ^= LED0; // P1.0 = toggle

    if (event.l == led_1)
        P1OUT ^= LED0; // P1.0 = toggle

    /* Now we check if the constrain is met, if not we have to
     *  take an action, preferably to push an error event to a
     *  queue.
     */
    if (ab_timing_check_criteria_failed(&event.ab, 10))
    {
        /* failed, the whole processing took more than the allowed 1 ms
         * lets toggle the other LED to indicate that. */
        P1OUT ^= LED1; // P1.0 = toggle
    }

    return 0;
}
