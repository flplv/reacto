#ifndef EVENT_LOOP_TYPES_H_
#define EVENT_LOOP_TYPES_H_

#include <reusables/fast_ring_fifo.h>
#include <reusables/linked_list.h>
#include <reusables/signal_slot.h>

struct main_loop_private;
typedef struct main_loop_private main_loop_t;

struct queue_private;
typedef struct queue_private queue_t;

struct queue_interface_private;
typedef struct queue_interface_private queue_i;

struct timed_queue_private;
typedef struct timed_queue_private timed_queue_t;
typedef struct timed_event_private timed_event_t;

struct slot_queue_private;
typedef struct slot_queue_private slot_queue_t;

struct signal_queue_private;
typedef struct signal_queue_private signal_queue_t;

typedef bool (*main_loop_strategy)(queue_i * queue);
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

struct queue_interface_private
{
    main_loop_t * loop;
    linked_list_t ll;
    bool (*emitter)(queue_i *);
    size_t (*count)(queue_i *);
    size_t (*hash)(queue_i *);
    void (*pop)(queue_i *);
};

struct queue_private
{
    signal_queue_t signal;
    fast_ring_fifo_t fifo;
    struct queue_interface_private itf;
};

struct timed_queue_private
{
    struct queue_private queue;
    timed_event_t * root;
    size_t cnt_cache;
    struct queue_interface_private itf;
};

struct main_loop_private
{
    queue_i * root;
    main_loop_strategy strategy;
    signal_t sleep;
    bool looping;
};

#endif /* EVENT_LOOP_TYPES_H_ */
