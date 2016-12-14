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
