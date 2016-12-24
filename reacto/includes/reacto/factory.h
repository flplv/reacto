#ifndef REACTO_FACTORY_H_
#define REACTO_FACTORY_H_

#ifndef REACTO_N_OF_QUEUES
#error "You have to define REACTO_N_OF_QUEUES with the number of queues you want to be allocated."
#endif

#ifndef REACTO_MAX_N_OF_HANDLERS_PER_QUEUE
#error "You have to define REACTO_MAX_N_OF_HANDLERS_PER_QUEUE with the maximum number of handlers per queue allowed."
#endif

#include <stdbool.h>
#include <stddef.h>
#include <reacto/queue.h>
#include <reacto/timed_queue.h>
#include <reacto/main_loop.h>
#include <reacto/reusables/debug.h>

struct reacto_queues_context
{
    slot_queue_handler_t queue_handlers[REACTO_N_OF_QUEUES][REACTO_MAX_N_OF_HANDLERS_PER_QUEUE];
    size_t queue_buffer_sizes[REACTO_N_OF_QUEUES];
};

struct reacto_timed_queue_context
{
    bool high_priority;
};

struct reacto_main_loop_context
{
    const main_loop_strategy strategy;
};

struct reacto_factory_private
{
    slot_queue_t slots [REACTO_N_OF_QUEUES][REACTO_MAX_N_OF_HANDLERS_PER_QUEUE];
    queue_t queues [REACTO_N_OF_QUEUES];
    timed_queue_t timed_queue;
    main_loop_t loop;

    bool has_loop;
    bool has_tq;
    size_t total_queues;
};

extern struct reacto_factory_private reacto_factory_private_data;

static inline main_loop_t * reacto_factory_main_loop()
{
    if (!reacto_factory_private_data.has_loop)
        return NULL;

    return &reacto_factory_private_data.loop;
}

static inline timed_queue_t * reacto_factory_timed_queue()
{
    if (!reacto_factory_private_data.has_tq)
        return NULL;

    return &reacto_factory_private_data.timed_queue;
}

static inline queue_t * reacto_factory_queue(size_t n)
{
    if (n >= reacto_factory_private_data.total_queues)
        return NULL;

    return &reacto_factory_private_data.queues[n];
}


#endif /* REACTO_FACTORY_H_ */
