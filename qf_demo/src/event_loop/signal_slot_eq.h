#ifndef SIGNALSLOT_EQ_H_
#define SIGNALSLOT_EQ_H_

#include <stdbool.h>
#include <stdint.h>
#include "event_loop_types.h"

void signal_eq_init(signal_eq_t *);
void signal_eq_deinit(signal_eq_t *);
bool signal_eq_is_connected(signal_eq_t *, slot_eq_t *);
void signal_eq_emit(signal_eq_t *, event_queue_t * queue);

void slot_eq_init(slot_eq_t *, slot_eq_handler_t handler);
void slot_eq_deinit(slot_eq_t *);
void slot_eq_connect(slot_eq_t *, signal_eq_t *);
int slot_eq_disconnect(slot_eq_t *, signal_eq_t *);


#endif /* SIGNALSLOT_EVENT_H_ */
