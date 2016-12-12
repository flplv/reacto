#include <reusables/checks.h>
#include "event_loop.h"
#include "signal_slot_eq.h"

static void _free (event_queue_t * queue)
{
    queue->loop = NULL;
    linked_list_init(queue, ll);
}

void event_loop_init(event_loop_t * obj, event_loop_strategy strategy)
{
    check_ptr(obj);
    check_ptr(strategy);

    obj->root = NULL;
    obj->strategy = strategy;
}

void event_loop_deinit(event_loop_t * obj)
{
    check_ptr(obj);

    if (obj->root)
        linked_list_free(obj->root, ll, _free);

    obj->root = NULL;
    obj->strategy = NULL;
}

void event_loop_add_queue(event_loop_t * obj, event_queue_t * queue, int position)
{
    check_ptr(obj);
    check_ptr(queue);
    event_queue_t * reference_queue;
    bool insert_after = false;

    if (!obj->root)
    {
        obj->root = queue;
        queue->loop = obj;
        return;
    }

    reference_queue = obj->root;

    while(position-- > 0)
    {
        event_queue_t * next = linked_list_next(reference_queue, ll);
        if (!next) {
            /* List ended before position, insert last */
            insert_after = true;
            break;
        }
        reference_queue = next;
    }

    if (insert_after)
    {
        linked_list_insert_after(reference_queue, queue, ll);
    }
    else
    {
        linked_list_insert_before(reference_queue, queue, ll);
        obj->root = linked_list_first(queue, ll);
    }
    queue->loop = obj;
}

int event_loop_remove_queue(event_loop_t * obj, event_queue_t * queue)
{
    check_ptr(obj, -1);
    check_ptr(queue, -1);

    if (queue->loop != obj) {
        log_error ("Provided event_queue hasn't been added to the event_loop, cannot remove it.");
        return -2;
    }

    obj->root = linked_list_remove(queue, ll);
    return 0;
}

void event_loop_process(event_loop_t * obj)
{
    check_ptr(obj);
    check_ptr(obj->strategy);

    if (obj->root)
        obj->strategy(obj->root);
}

static void process (event_queue_t * queue, bool all)
{
    while (queue)
    {
        size_t cnt;
        while ((cnt = fast_ring_fifo_count(&queue->fifo)) != 0)
        {
            size_t rd_pos = fast_ring_fifo_read_index(&queue->fifo);

            signal_eq_emit(&queue->signal, queue);

            /* If the queue data has not been popped out, we discard it here
             * If a handler pop out data, others handlers will loose access to
             * it. */
            if (rd_pos == fast_ring_fifo_read_index(&queue->fifo))
                fast_ring_fifo_read_increment(&queue->fifo);

            if (!all)
                return;
        }
        queue = linked_list_next(queue, ll);
    }
}

static void all_at_once (event_queue_t * queue)
{
    process(queue, true);
}

static void only_one (event_queue_t * queue)
{
    process(queue, false);
}

const event_loop_strategy event_loop_strategy_consume_all_at_once = all_at_once;
const event_loop_strategy event_loop_strategy_consume_only_one_per_loop = only_one;
