/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Felipe Lavratti
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef REACTO_WATCHDOG_WATCHDOG_H_
#define REACTO_WATCHDOG_WATCHDOG_H_

/*
 * This is a Software Watchdog Module
 * User code declare and initialize watchdog objects
 * that will be checked when feeding the hardware watchdog.
 * If any software watchdog expires, no hardware feeding will occur.
 */

#include "../reusables/linked_list.h"
#include "../reusables/timeout.h"

struct _watchdog_private
{
    const char * name;
    reacto_time_t period;
    timeout_t timeout;
    linked_list_t list_head;
};

typedef struct _watchdog_private watchdog_t;

/* Initialize a soft watchdog object.
 * period: the amount of time to the watchdog expire
 * name: the name of the watchdog, must be a string literal or global.
 */
int watchdog_init(watchdog_t *, reacto_time_t period, const char * name);
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

#endif /* REACTO_WATCHDOG_WATCHDOG_H_ */
