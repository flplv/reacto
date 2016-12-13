#include "queue.h"

#include <reusables/checks.h>
#include <event_loop/main_loop.h>
#include <event_loop/signal_slot_queue.h>
#include <reusables/log.h>

int queue_init(queue_t * obj, size_t number_of_slots)
{
    int r;
    check_ptr(obj, -1);

    obj->loop = NULL;
    linked_list_init(obj, ll);
    signal_queue_init(&obj->signal);

    r = fast_ring_fifo_init(&obj->fifo, number_of_slots);

    if (r == -1)
    {
        log_error("Unable to initialize Fast Ring FIFO, size is too small");
        return -1;
    }

    return r;
}

void queue_deinit(queue_t * obj)
{
    check_ptr(obj);

    if (obj->loop)
        main_loop_remove_queue(obj->loop, obj);

    fast_ring_fifo_init(&obj->fifo, 0);
    signal_queue_deinit(&obj->signal);
    obj->loop = NULL;
}

bool queue_full(queue_t * obj)
{
    check_ptr(obj, false);
    return fast_ring_fifo_full(&obj->fifo);
}

signal_queue_t * queue_signal(queue_t * obj)
{
    check_ptr(obj, NULL);
    return &obj->signal;
}
