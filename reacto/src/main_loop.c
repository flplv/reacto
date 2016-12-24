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
#include <reacto/reusables/debug.h>
#include <reacto/reusables/log.h>
#include <reacto/main_loop.h>
#include <reacto/signal_slot_queue.h>
#include <reacto/queue_interface.h>

static void _free (queue_i * queue)
{
    queue->loop = NULL;
    linked_list_init(queue, ll);
}

void main_loop_init(main_loop_t * obj, main_loop_strategy strategy)
{
    debug_ptr(obj);
    debug_ptr(strategy);

    obj->root = NULL;
    obj->strategy = strategy;
    obj->looping = true;
    obj->sleep = NULL;
}

void main_loop_deinit(main_loop_t * obj)
{
    debug_ptr(obj);

    if (obj->root)
        linked_list_free(obj->root, ll, _free);

    obj->root = NULL;
    obj->strategy = NULL;
    obj->sleep = NULL;
}

void main_loop_set_sleep_handler (main_loop_t * obj, void (*handler)(main_loop_t *))
{
    debug_ptr(obj);
    obj->sleep = handler;
}

void main_loop_add_queue(main_loop_t * obj, queue_i * queue, int position)
{
    debug_ptr(obj);
    debug_ptr(queue);
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
    debug_ptr(obj, -1);
    debug_ptr(queue, -1);

    if (queue->loop != obj) {
        log_error ("Provided queue hasn't been added to the main_loop, cannot remove it.");
        return -2;
    }

    obj->root = linked_list_remove(queue, ll);
    return 0;
}

void main_loop_run(main_loop_t * obj)
{
    debug_ptr(obj);
    debug_ptr(obj->strategy);

    do
    {
        bool go_to_sleep = obj->strategy(obj->root);
        if (go_to_sleep && obj->sleep)
            obj->sleep(obj);
    }
    while (obj->looping);
}

void main_loop_quit(main_loop_t * obj)
{
    debug_ptr(obj);
    obj->looping = false;
}

uint32_t main_loop_sleep_timeout (main_loop_t * obj)
{
    debug_ptr(obj, false);
    queue_i * queue = obj->root;
    uint32_t tout = UINT32_MAX;

    while (queue)
    {
        uint32_t n = queue_interface_sleep_tout(queue);
        if (n < tout)
            tout = n;

        queue = linked_list_next(queue, ll);
    }

    return tout;
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
