#include <msp430.h>
#include <legacymsp430.h>
#include <reacto/main_loop.h>
#include <reacto/queue.h>

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

typedef enum led
{
    led_invalid,
    led_0
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

static int button_stream_handler (queue_t * queue);
static int led_stream_handler (queue_t * queue);

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

    if (event == button_0)
        queue_push (&led_stream.queue, led_stream.buffer, led_0);

    return 0;
}

/* Third the led stream handler */
static int led_stream_handler (queue_t * queue)
{
    led_event_t event = led_invalid;
    queue_peek (&led_stream.queue, led_stream.buffer, &event);

    if (event == led_0)
        P1OUT ^= (LED0 + LED1); // P1.0 = toggle

    return 0;
}
