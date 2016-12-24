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
#include <reacto/event_loop_types.h>

bool queue_interface_emit(queue_i * itf)
{
    debug_ptr(itf, false);
    debug_ptr(itf->emitter, false);
    return itf->emitter(itf);
}

size_t queue_interface_count(queue_i * itf)
{
    debug_ptr(itf, false);
    debug_ptr(itf->count, false);
    return itf->count(itf);
}

uint32_t queue_interface_sleep_tout(queue_i * itf)
{
    debug_ptr(itf, false);
    debug_ptr(itf->count, false);
    return itf->sleep(itf);
}

size_t queue_interface_hash(queue_i * itf)
{
    debug_ptr(itf, false);
    debug_ptr(itf->count, false);
    return itf->hash(itf);
}

void queue_interface_pop(queue_i * itf)
{
    debug_ptr(itf);
    debug_ptr(itf->pop);
    itf->pop(itf);
}
