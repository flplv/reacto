#ifndef EVENT_LOOP_SIGNALSLOT_QUEUE_H_
#define EVENT_LOOP_SIGNALSLOT_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>
#include "event_loop_types.h"

void signal_queue_init(signal_queue_t *);
void signal_queue_deinit(signal_queue_t *);
bool signal_queue_is_connected(signal_queue_t *, slot_queue_t *);
void signal_queue_emit(signal_queue_t *, queue_t * queue);

void slot_queue_init(slot_queue_t *, slot_queue_handler_t handler);
void slot_queue_deinit(slot_queue_t *);
void slot_queue_connect(slot_queue_t *, signal_queue_t *);
int slot_queue_disconnect(slot_queue_t *, signal_queue_t *);


#endif /* SIGNALSLOT_EVENT_H_ */
