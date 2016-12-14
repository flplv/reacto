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
#include <reusables/checks.h>
#include <reusables/macros.h>
#include <reusables/log.h>
#include <queue.h>
#include <queue_interface.h>
#include <main_loop.h>
#include <signal_slot_queue.h>

static bool emitter(queue_i * itf)
{
    queue_t * obj = container_of(itf, typeof(*obj), itf);
    signal_queue_emit(&obj->signal, obj);
    return false;
}

static size_t count(queue_i * itf)
{
    queue_t * obj = container_of(itf, typeof(*obj), itf);
    return fast_ring_fifo_count(&obj->fifo);
}

static size_t hash(queue_i * itf)
{
    queue_t * obj = container_of(itf, typeof(*obj), itf);
    return fast_ring_fifo_read_index(&obj->fifo);
}

static void pop(queue_i * itf)
{
    queue_t * obj = container_of(itf, typeof(*obj), itf);
    fast_ring_fifo_read_increment(&obj->fifo);
}

int queue_init(queue_t * obj, size_t number_of_slots)
{
    int r;
    check_ptr(obj, -1);

    obj->itf.loop = NULL;
    obj->itf.emitter = emitter;
    obj->itf.count = count;
    obj->itf.hash = hash;
    obj->itf.pop = pop;
    linked_list_init(&obj->itf, ll);
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

    if (obj->itf.loop)
        main_loop_remove_queue(obj->itf.loop, &obj->itf);

    fast_ring_fifo_init(&obj->fifo, 0);
    signal_queue_deinit(&obj->signal);
    obj->itf.loop = NULL;
}

signal_queue_t * queue_signal(queue_t * obj)
{
    check_ptr(obj, NULL);
    return &obj->signal;
}

queue_i * queue_interface (queue_t * obj)
{
    check_ptr(obj, NULL);
    return &obj->itf;
}
