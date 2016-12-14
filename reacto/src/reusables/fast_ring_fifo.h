#ifndef FAST_RING_FIFO_H_
#define FAST_RING_FIFO_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct s_fast_ring_fifo_private {
    size_t mask;
    size_t wrIdx;
    size_t rdIdx;
} fast_ring_fifo_t;

/*
 * num_of_slots must be base of two, i.e. 2, 4, 8, 16, 32, 64, ...
 */
static inline int fast_ring_fifo_init(fast_ring_fifo_t * obj, size_t num_of_slots)
{
    size_t pre_mask = 0x80000000;
    while (pre_mask)
    {
        if (num_of_slots & pre_mask) {
            break;
        }

        pre_mask >>= 1;
    }

    if (!pre_mask) {
        obj->mask = 0;
        return -1;
    }

    obj->mask = pre_mask - 1;
    obj->rdIdx = 0;
    obj->wrIdx = 0;

    return (int)pre_mask;
}

static inline size_t fast_ring_fifo_num_of_slots(fast_ring_fifo_t * obj)
{
    return obj->mask + 1;
}

static inline size_t fast_ring_fifo_write_index(fast_ring_fifo_t * obj)
{
    return obj->mask & obj->wrIdx;
}

static inline size_t fast_ring_fifo_read_index(fast_ring_fifo_t * obj)
{
    return obj->mask & obj->rdIdx;
}

static inline void fast_ring_fifo_write_increment(fast_ring_fifo_t * obj)
{
    obj->wrIdx++;
}

static inline void fast_ring_fifo_read_increment(fast_ring_fifo_t * obj)
{
    obj->rdIdx++;
}

static inline void fast_ring_fifo_read_reset(fast_ring_fifo_t * obj)
{
    obj->rdIdx = 0;
    obj->wrIdx = 0;
}

static inline bool fast_ring_fifo_empty(fast_ring_fifo_t * obj)
{
    return obj->rdIdx == obj->wrIdx;
}

static inline bool fast_ring_fifo_full(fast_ring_fifo_t * obj)
{
    return (obj->mask & obj->rdIdx) == (obj->mask & (obj->wrIdx+1));
}

static inline size_t fast_ring_fifo_count(fast_ring_fifo_t * obj)
{
    return obj->mask & (obj->wrIdx - obj->rdIdx);
}

#endif /* FAST_RING_FIFO_H_ */
