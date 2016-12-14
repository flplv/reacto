#ifndef TIMED_QUEUE_H_
#define TIMED_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>
#include <reusables/linked_list.h>
#include "event_loop_types.h"

/*
 * This is the Timed Queue, to be used with the Main Loop.
 * You can schedule deferred handlers to be called in the configured timestamp.
 */

typedef void (*timed_event_handler_t)(timed_event_t *);

struct timed_event_private
{
    uint32_t timestamp;
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
 */
void timed_queue_link(timed_queue_t * obj, timed_event_t * ev);

queue_i * timed_queue_interface (timed_queue_t * obj);

#endif /* TIMED_QUEUE_H_ */
