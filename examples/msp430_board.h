

#ifndef interrupt
#define interrupt(...) void
#endif

#define LED0 BIT0
#define LED1 BIT6
#define BUTTON BIT3

static void board_init(void)
{
    __disable_interrupt();
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;     // Set range
    DCOCTL = CALDCO_1MHZ;      // SMCLK = DCO = 1MHz
    CCTL0 = CCIE;
    TACTL = TASSEL_2 + MC_1; // Set the timer A to SMCLCK, Continuous
    TACCR0 = 1000;
    // Clear the timer and enable timer interrupt

    P1DIR |= (LED0 + LED1); // Set P1.0 to output direction
    // P1.3 must stay at input
    P1OUT |= (LED0 + LED1);
    P1IE |= BUTTON; // P1.3 interrupt enabled
    P1IFG &= ~BUTTON; // P1.3 IFG cleared
}

volatile static uint32_t time_ms_cnt;

interrupt(TIMER0_A0_VECTOR) timer_isr (void)
{
    time_ms_cnt++;
}

/* Examples uses the time.h module, it is necessary to define
   the platform dependent functions */
uint32_t time_now_ms ()
{
    return time_ms_cnt;
}

void time_sleep (uint32_t delay)
{
    volatile const uint32_t now = time_ms_cnt;
    while ((time_ms_cnt - now) < delay);
}
