#ifndef EVENT_LOOP_TYPES_H_
#define EVENT_LOOP_TYPES_H_

#include <reusables/fast_ring_fifo.h>
#include <reusables/linked_list.h>

struct event_loop_private;
typedef struct event_loop_private event_loop_t;

struct event_queue_private;
typedef struct event_queue_private event_queue_t;

struct slot_eq_private;
typedef struct slot_eq_private slot_eq_t;

struct signal_eq_private;
typedef struct signal_eq_private signal_eq_t;

typedef int (*slot_eq_handler_t) (event_queue_t * queue);

typedef void (*event_loop_strategy)(event_queue_t * queue);

struct slot_eq_private
{
    signal_eq_t * connection;
    slot_eq_handler_t handler;
    linked_list_t ll;
};

struct signal_eq_private
{
    slot_eq_t * root;
};

struct event_queue_private
{
    event_loop_t * loop;
    signal_eq_t signal;
    fast_ring_fifo_t fifo;
    linked_list_t ll;
};

struct event_loop_private
{
    event_queue_t * root;
    event_loop_strategy strategy;
};

#endif /* EVENT_LOOP_TYPES_H_ */
