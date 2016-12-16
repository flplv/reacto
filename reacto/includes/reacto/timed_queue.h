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
#ifndef REACTO_TIMED_QUEUE_H_
#define REACTO_TIMED_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>
#include "reusables/linked_list.h"
#include "event_loop_types.h"

/*
 * This is the Timed Queue, to be used with the Main Loop.
 * You can schedule deferred handlers to be called in the configured timestamp.
 */

typedef void (*timed_event_handler_t)(timed_event_t *);

struct timed_event_private
{
    uint32_t link_timestamp;
    uint32_t timeout;
    timed_event_handler_t handler;
    linked_list_t ll;
};
void timed_event_init(timed_event_t * ev, uint32_t timeout, timed_event_handler_t handler);

void timed_queue_init(timed_queue_t * obj);
void timed_queue_deinit(timed_queue_t * obj);

/*
 * Whenever you link a timed_event_t to the queue, you must keep it in valid scope until
 * the handler is called, zero copy here, it is only linked.
 * NOT ISR SAFE
 * The configured timeout in the event starts counting after linking.
 * It is safe to re link without unlinking.
 */
void timed_queue_link(timed_queue_t * obj, timed_event_t * ev);
/*
 * Same as last, but now you can change event timeout.
 */
void timed_queue_link_update_timeout(timed_queue_t * obj, timed_event_t * ev, uint32_t timeout);
/*
 * Unlinking an event prevent the handler from being called.
 * When a handler is called, the event is automatically unlinked.
 */
void timed_queue_unlink(timed_queue_t * obj, timed_event_t * ev);

queue_i * timed_queue_interface (timed_queue_t * obj);

#endif /* REACTO_TIMED_QUEUE_H_ */
