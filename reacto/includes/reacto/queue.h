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
#ifndef REACTO_QUEUE_H_
#define REACTO_QUEUE_H_

#include <stddef.h>
#include "event_loop_types.h"
#include "signal_slot_queue.h"

/*
 * Number of slots must be a power of two, otherwise it will be down sized
 *  to its approximate power of two.
 * The fast_ring_fifo algorithm used by this queue cannot fill the whole
 *  buffer, it becomes full with number_of_slots - 1. If you need a queue
 *  of two slots, make it 4, if you need 3, make it 8, and so on.
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

#define queue_full(queue) \
({ \
    fast_ring_fifo_t * _fifo = &((queue)->fifo); \
    bool _full = false; \
    if (fast_ring_fifo_full (_fifo)) {\
        _full = true; \
    } \
    _full; \
})

signal_queue_t * queue_signal(queue_t * obj);
queue_i * queue_interface (queue_t * obj);

#endif /* REACTO_QUEUE_H_ */
