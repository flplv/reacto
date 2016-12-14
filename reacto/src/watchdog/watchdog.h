#ifndef WATCHDOG_H_
#define WATCHDOG_H_

/*
 * This is a Software Watchdog Module
 * User code declare and initialize watchdog objects
 * that will be checked when feeding the hardware watchdog.
 * If any software watchdog expires, no hardware feeding will occur.
 */

#include <reusables/linked_list.h>
#include <reusables/timeout.h>

struct _watchdog_private
{
    const char * name;
    uint32_t period_ms;
    timeout_t timeout;
    linked_list_t list_head;
};

typedef struct _watchdog_private watchdog_t;

/* Initialize a soft watchdog object.
 * period_ms: the amount of time to the watchdog expire
 * name: the name of the watchdog, must be a string literal or global.
 */
int watchdog_init(watchdog_t *, uint32_t period_ms, const char * name);
void watchdog_deinit(watchdog_t *);

/* ISR safe */
void watchdog_enter (watchdog_t * obj);

/* ISR safe */
void watchdog_exit (watchdog_t * obj);

/* ISR safe */
void watchdog_kick (watchdog_t * obj);

/*
 * Call this periodically,
 * this function will kick the hardware watchdog
 * only if every active soft watchdog has not expired.
 *
 * Configure hardware watchdog timeout to match the
 * frequency this function is called.
 *
 * ISR safe
 */
void watchdog_periodic ();

#endif /* WATCHDOG_H_ */
