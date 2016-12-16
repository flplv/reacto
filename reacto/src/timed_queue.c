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
#include <reacto/reusables/macros.h>
#include <reacto/reusables/log.h>
#include <reacto/reusables/debug.h>
#include <reacto/reusables/time.h>
#include <reacto/reusables/timeout.h>
#include <reacto/timed_queue.h>
#include <reacto/main_loop.h>
#include <reacto/signal_slot_queue.h>

static void unlink(timed_queue_t * obj, timed_event_t * ev)
{
    obj->root = linked_list_remove(ev, ll);
    linked_list_init(ev, ll);
    obj->cnt_cache--;
}

static bool emitter(queue_i * itf)
{
    timed_queue_t * obj = container_of(itf, typeof(*obj), itf);
    timed_event_t * ev = obj->root;

    if (!timeout_check_elapsed(time_now_ms(), ev->link_timestamp, ev->timeout))
        return true; /* Skip this queue */

    unlink(obj, ev);
    ev->handler(ev);
    return false;
}

static size_t count(queue_i * itf)
{
    timed_queue_t * obj = container_of(itf, typeof(*obj), itf);
    return obj->cnt_cache;
}

static size_t hash(queue_i * obj)
{
    obj = obj + 1;
    return 0;
}

static void pop(queue_i * obj)
{
    obj = obj + 1;
}

void timed_queue_init(timed_queue_t * obj)
{
    debug_ptr(obj);

    obj->itf.loop = NULL;
    obj->itf.emitter = emitter;
    obj->itf.pop = pop;
    obj->itf.count = count;
    obj->itf.hash = hash;
    linked_list_init(&obj->itf, ll);
    obj->root = NULL;
    obj->cnt_cache = 0;
}

static void _free (timed_event_t * ev)
{
    linked_list_init(ev, ll);
}

void timed_queue_deinit(timed_queue_t * obj)
{
    debug_ptr(obj);

    if (obj->itf.loop)
        main_loop_remove_queue(obj->itf.loop, &obj->itf);

    obj->itf.loop = 0;
    linked_list_free(obj->root, ll, _free);
    obj->root = NULL;
    obj->cnt_cache = 0;
}

void timed_event_init(timed_event_t * ev, uint32_t timeout, timed_event_handler_t handler)
{
    debug_ptr(ev);
    debug_ptr(handler);
    ev->link_timestamp = 0;
    ev->timeout = timeout;
    ev->handler = handler;
    linked_list_init(ev, ll);
}

static bool comp (uint32_t seed, timed_event_t * ev)
{
    return (ev->link_timestamp + ev->timeout) > seed ? true : false;
}

void timed_queue_unlink(timed_queue_t * obj, timed_event_t * ev)
{
    debug_ptr(obj);
    debug_ptr(ev);

    if (ev->ll.next == 0 && ev->ll.prev == 0 && obj->root != ev)
        return;

    unlink(obj, ev);
}

void timed_queue_link(timed_queue_t * obj, timed_event_t * ev)
{
    debug_ptr(obj);
    debug_ptr(ev);

    timed_queue_unlink(obj, ev);

    ev->link_timestamp = time_now_ms();


    if (!obj->root)
    {
        obj->root = ev;
    }
    else
    {
        timed_event_t * after = linked_list_find(obj->root, ll, comp, ev->link_timestamp + ev->timeout);

        if (after)
        {
            linked_list_insert_before(after, ev, ll);
            obj->root = linked_list_first(ev, ll);
        }
        else
        {
            linked_list_insert_after(linked_list_last(obj->root, ll), ev, ll);
        }
    }

    obj->cnt_cache++;
}


void timed_queue_link_update_timeout(timed_queue_t * obj, timed_event_t * ev, uint32_t timeout)
{
    debug_ptr(obj);
    debug_ptr(ev);

    ev->timeout = timeout;
    timed_queue_link(obj, ev);
}

queue_i * timed_queue_interface (timed_queue_t * obj)
{
    debug_ptr(obj, NULL);
    return &obj->itf;
}
