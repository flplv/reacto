#ifndef QUEUE_INTERFACE_H_
#define QUEUE_INTERFACE_H_

#include <stdbool.h>
#include "event_loop_types.h"

bool queue_interface_emit(queue_i * itf);
size_t queue_interface_count(queue_i * itf);
void queue_interface_pop(queue_i * itf);
size_t queue_interface_hash(queue_i * itf);

#endif /* QUEUE_INTERFACE_H_ */
