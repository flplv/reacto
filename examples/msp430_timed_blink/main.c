#include <msp430.h>
#include <legacymsp430.h>
#include <main_loop.h>
#include <queue.h>
#include <timed_queue.h>

#define LED0 BIT0
#define LED1 BIT6
#define BUTTON BIT3

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
static uint8_t button_0_pressed_cnt;

static int button_stream_handler (queue_t * queue);
static void delayed_handler (timed_event_t * ev);
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

static void time_stream_init()
{
    timed_queue_init(&time_stream.queue);
    main_loop_add_queue(&loop, timed_queue_interface(&time_stream.queue), 0);
    timed_event_init(&time_stream.event, 250, delayed_handler);
}

static void board_init(void)
{
    __disable_interrupt();
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;     // Set range
    DCOCTL = CALDCO_1MHZ;      // SMCLK = DCO = 1MHz
    CCTL0 = CCIE;
    TACTL = TASSEL_2 + MC_2; // Set the timer A to SMCLCK, Continuous
    // Clear the timer and enable timer interrupt
    P1DIR |= (LED0 + LED1); // Set P1.0 to output direction
    // P1.3 must stay at input
    P1OUT |= (LED0 + LED1);
    P1IE |= BUTTON; // P1.3 interrupt enabled
    P1IFG &= ~BUTTON; // P1.3 IFG cleared
}

int main (void)
{
    board_init();

    main_loop_init (&loop, main_loop_strategy_fare);
    button_stream_init();
    led_stream_init();
    time_stream_init();
    button_0_pressed_cnt = 0;

    __enable_interrupt(); // enable all interrupts
    // __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

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

    if (!timed_event_is_linked(&time_stream.event))
    {
        /* Starts the 250ms job */
        timed_queue_link(&time_stream.queue, &time_stream.event);
    }

    if (event == button_0)
    {
        /* Increment the counter */
        button_0_pressed_cnt ++;

        /* Push to the queue a led_0 event */
        queue_push (&led_stream.queue, led_stream.buffer, led_0);
    }

    return 0;
}

/* Third the timed event handler */
static void delayed_handler (timed_event_t * ev)
{
    button_event_t event = button_invalid;
    queue_peek (&button_stream.queue, button_stream.buffer, &event);

    if (button_0_pressed_cnt > 1)
        /* Push to the queue a led_1 event if multiple clicked */
        queue_push (&led_stream.queue, led_stream.buffer, led_1);

    button_0_pressed_cnt = 0;
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


static uint32_t time_ms_cnt;

interrupt(TIMER0_A1_VECTOR) timer_isr (void)
{
    static uint16_t pre_cnt;
    pre_cnt ++;

    if (pre_cnt > 1000)
    {
        pre_cnt = 0;
        time_ms_cnt++;
    }
}

/* Timed Queue uses the time.h module, it is necessary to define
   the platform dependent functions */
uint32_t time_now_ms ()
{
    return time_ms_cnt;
}

void time_sleep (uint32_t delay)
{
    volatile uint32_t now = time_ms_cnt;
    while ((time_ms_cnt - now) < delay);
}
