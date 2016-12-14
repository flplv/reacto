#include <reusables/checks.h>
#include "signal_slot_queue.h"
#include "main_loop.h"
#include "queue_interface.h"

static void _free (queue_i * queue)
{
    queue->loop = NULL;
    linked_list_init(queue, ll);
}

void main_loop_init(main_loop_t * obj, main_loop_strategy strategy)
{
    check_ptr(obj);
    check_ptr(strategy);

    obj->root = NULL;
    obj->strategy = strategy;
    obj->looping = true;
    signal_init(&obj->sleep);
}

void main_loop_deinit(main_loop_t * obj)
{
    check_ptr(obj);

    if (obj->root)
        linked_list_free(obj->root, ll, _free);

    obj->root = NULL;
    obj->strategy = NULL;
    signal_deinit(&obj->sleep);
}

signal_t * main_loop_sleep_signal (main_loop_t * obj)
{
    check_ptr(obj, NULL);
    return &obj->sleep;
}

void main_loop_add_queue(main_loop_t * obj, queue_i * queue, int position)
{
    check_ptr(obj);
    check_ptr(queue);
    queue_i * reference_queue;
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
        queue_i * next = linked_list_next(reference_queue, ll);
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

int main_loop_remove_queue(main_loop_t * obj, queue_i * queue)
{
    check_ptr(obj, -1);
    check_ptr(queue, -1);

    if (queue->loop != obj) {
        log_error ("Provided queue hasn't been added to the main_loop, cannot remove it.");
        return -2;
    }

    obj->root = linked_list_remove(queue, ll);
    return 0;
}

void main_loop_run(main_loop_t * obj)
{
    check_ptr(obj);
    check_ptr(obj->strategy);

    do
    {
        bool go_to_sleep = obj->strategy(obj->root);
        if (go_to_sleep)
            signal_emit(&obj->sleep);
    }
    while (obj->looping);
}

void main_loop_quit(main_loop_t * obj)
{
    check_ptr(obj);
    obj->looping = false;
}

bool main_loop_ready_to_sleep (main_loop_t * obj)
{
    check_ptr(obj, false);
    queue_i * queue = obj->root;

    while (queue)
    {
        if (queue_interface_count(queue) != 0)
            return false;

        queue = linked_list_next(queue, ll);
    }

    return true;
}

static bool priority_queue_and_fare (queue_i * queue, bool priority_enabled)
{
    while (queue)
    {
        size_t cnt;
        while ((cnt = queue_interface_count(queue)) != 0)
        {
            size_t hash = queue_interface_hash(queue);
            bool skip_queue = queue_interface_emit(queue);

            if (skip_queue)
                break;

            /* If the queue data has not been popped out, we discard it here
             * If a handler pop out data, others handlers will loose access to
             * it. */
            if (hash == queue_interface_hash(queue))
                queue_interface_pop(queue);

            /* Queues are not empty yet */
            if (priority_enabled)
                return false;
        }
        queue = linked_list_next(queue, ll);
    }

    /* Maybe queues are empty */
    return true;
}

static bool priority_queue(queue_i * queue)
{
    return priority_queue_and_fare (queue, true);
}

static bool fare(queue_i * queue)
{
    return priority_queue_and_fare (queue, false);
}

const main_loop_strategy main_loop_strategy_priority_queue = priority_queue;
const main_loop_strategy main_loop_strategy_fare = fare;
