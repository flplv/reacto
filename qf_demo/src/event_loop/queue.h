#ifndef EVENT_LOOP_QUEUE_H_
#define EVENT_LOOP_QUEUE_H_

#include <stddef.h>
#include "event_loop_types.h"

/*
 * Number of slots must be a power of two, otherwise it will be down sized
 * to its approximate power of two.
 * `event_queue_init` returns -1 if error or the accepted number_of_slots.
 */
int queue_init(queue_t * obj, size_t number_of_slots);
void queue_deinit(queue_t * obj);

/*
 * ISR safe, use this function to add data to the queue.
 */
#define queue_push(queue, buffer, data)   \
({ \
    fast_ring_fifo_t * _fifo = &((queue)->fifo); \
    int _success = -1; \
    if (!fast_ring_fifo_full (_fifo)) {\
        buffer[fast_ring_fifo_write_index(_fifo)] = data; \
        fast_ring_fifo_write_increment(_fifo); \
        _success = 0; \
    } \
    _success; \
})

/*
 * Use this function to get data from the queue in the slot handler.
 */
#define queue_peek(queue, buffer, destination)   \
({ \
    fast_ring_fifo_t * _fifo = &((queue)->fifo); \
    int _success = -1; \
    if (!fast_ring_fifo_empty (_fifo)) {\
        *(destination) = buffer[fast_ring_fifo_read_index(_fifo)]; \
        _success = 0; \
    } \
    _success; \
})

bool queue_full(queue_t * obj);
signal_queue_t * queue_signal(queue_t * obj);


#endif /* EVENT_LOOP_QUEUE_H_ */
