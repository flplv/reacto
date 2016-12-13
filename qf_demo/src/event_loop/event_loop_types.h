#ifndef EVENT_LOOP_TYPES_H_
#define EVENT_LOOP_TYPES_H_

#include <reusables/fast_ring_fifo.h>
#include <reusables/linked_list.h>
#include <reusables/signal_slot.h>

struct main_loop_private;
typedef struct main_loop_private main_loop_t;

struct queue_private;
typedef struct queue_private queue_t;

struct slot_queue_private;
typedef struct slot_queue_private slot_queue_t;

struct signal_queue_private;
typedef struct signal_queue_private signal_queue_t;

typedef bool (*main_loop_strategy)(queue_t * queue);
typedef int (*slot_queue_handler_t)(queue_t * queue);

struct slot_queue_private
{
    signal_queue_t * connection;
    slot_queue_handler_t handler;
    linked_list_t ll;
};

struct signal_queue_private
{
    slot_queue_t * root;
};

struct queue_private
{
    main_loop_t * loop;
    signal_queue_t signal;
    fast_ring_fifo_t fifo;
    linked_list_t ll;
};

struct main_loop_private
{
    queue_t * root;
    main_loop_strategy strategy;
    signal_t sleep;
    bool looping;
};

#endif /* EVENT_LOOP_TYPES_H_ */
