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
#ifndef REACTO_REUSABLES_SIGNAL_SLOT_H_
#define REACTO_REUSABLES_SIGNAL_SLOT_H_

#include "linked_list.h"
#include <stdbool.h>

struct slot_private;
typedef struct slot_private slot_t;

struct signal_private;
typedef struct signal_private signal_t;

/*
 * Rules of the Handler:
 *  Handlers will be called in the order they were inserted.
 *  If a handler returns anything but 0, the call loop will stop and no other
 *   connected handler will be called.
 */
typedef int (*slot_handler_t) (slot_t *);

void signal_init(signal_t *);
void signal_deinit(signal_t *);
bool signal_is_connected(signal_t *, slot_t *);
void signal_emit(signal_t *);

void slot_init(slot_t *, slot_handler_t handler);
void slot_deinit(slot_t *);
void slot_connect(slot_t *, signal_t *);
int slot_disconnect(slot_t *, signal_t *);

struct signal_private
{
    struct slot_private * root;
};

struct slot_private
{
    signal_t * connection;
    slot_handler_t handler;
    linked_list_t ll;
};

#endif /* REACTO_REUSABLES_SIGNAL_SLOT_H_ */
