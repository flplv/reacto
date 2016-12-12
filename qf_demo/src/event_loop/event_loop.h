#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <stddef.h>
#include "event_loop_types.h"

/* All At Once Strategy will keep calling handlers until queues get empty */
extern const event_loop_strategy event_loop_strategy_consume_all_at_once;
/* Only One Per Loop Strategy will call handlers of only first queue that has data */
extern const event_loop_strategy event_loop_strategy_consume_only_one_per_loop;

void event_loop_init(event_loop_t * obj, event_loop_strategy strategy);

void event_loop_deinit(event_loop_t * obj);

void event_loop_add_queue(event_loop_t * obj, event_queue_t * queue, int position);
int event_loop_remove_queue(event_loop_t * obj, event_queue_t * queue);

void event_loop_process(event_loop_t * obj);

#endif /* EVENT_LOOP_H_ */
