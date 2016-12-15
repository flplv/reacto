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
#ifndef MAIN_LOOP_H_
#define MAIN_LOOP_H_

#include <stddef.h>
#include <reusables/signal_slot.h>
#include "event_loop_types.h"

/* First queues get prioritized */
extern const main_loop_strategy main_loop_strategy_priority_queue;
/* Queues get approximately the same importance */
extern const main_loop_strategy main_loop_strategy_fare;

void main_loop_init(main_loop_t * obj, main_loop_strategy strategy);
void main_loop_deinit(main_loop_t * obj);

void main_loop_add_queue(main_loop_t * obj, queue_i * queue, int position);
int main_loop_remove_queue(main_loop_t * obj, queue_i * queue);

void main_loop_run(main_loop_t * obj);
void main_loop_quit(main_loop_t * obj);

bool main_loop_ready_to_sleep (main_loop_t * obj);

void main_loop_set_sleep_handler (main_loop_t * obj, void (*handler)(main_loop_t *));

#endif /* MAIN_LOOP_H_ */
