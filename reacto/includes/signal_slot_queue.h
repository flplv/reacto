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
#ifndef SIGNAL_SLOT_QUEUE_H_
#define SIGNAL_SLOT_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>
#include "event_loop_types.h"

void signal_queue_init(signal_queue_t *);
void signal_queue_deinit(signal_queue_t *);
bool signal_queue_is_connected(signal_queue_t *, slot_queue_t *);
void signal_queue_emit(signal_queue_t *, queue_t * queue);

void slot_queue_init(slot_queue_t *, slot_queue_handler_t handler);
void slot_queue_deinit(slot_queue_t *);
void slot_queue_connect(slot_queue_t *, signal_queue_t *);
int slot_queue_disconnect(slot_queue_t *, signal_queue_t *);


#endif /* SIGNAL_SLOT_QUEUE_H_ */
