#include <reusables/checks.h>
#include <reusables/log.h>
#include "event_queue.h"
#include "event_loop.h"
#include "signal_slot_eq.h"

int event_queue_init(event_queue_t * obj, size_t number_of_slots)
{
    int r;
    check_ptr(obj, -1);

    obj->loop = NULL;
    linked_list_init(obj, ll);
    signal_eq_init(&obj->signal);

    r = fast_ring_fifo_init(&obj->fifo, number_of_slots);

    if (r == -1)
    {
        log_error("Unable to initialize Fast Ring FIFO, size is too small");
        return -1;
    }

    return r;
}

void event_queue_deinit(event_queue_t * obj)
{
    check_ptr(obj);

    if (obj->loop)
        event_loop_remove_queue(obj->loop, obj);

    fast_ring_fifo_init(&obj->fifo, 0);
    signal_eq_deinit(&obj->signal);
    obj->loop = NULL;
}

signal_eq_t * event_queue_signal(event_queue_t * obj)
{
    check_ptr(obj, NULL);
    return &obj->signal;
}
