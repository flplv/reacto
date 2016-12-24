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
#include <stdbool.h>
#include <reacto/reusables/timeout.h>
#include <reacto/reusables/time.h>

void timeout_init(timeout_t * cobj)
{
    *cobj = time_now();
}

void timeout_copy(timeout_t * to, timeout_t * from)
{
	*to = *from;
}

bool timeout_check_elapsed(reacto_time_t now, reacto_time_t before, reacto_time_t desired_wait)
{
    return (now - before) >= desired_wait;
}

reacto_time_t timeout_remaining(reacto_time_t now, reacto_time_t before, reacto_time_t desired_wait)
{
    if (timeout_check_elapsed(now, before, desired_wait - 1))
        return 0;

    return desired_wait - (now - before);
}

bool timeout_check_reached(reacto_time_t timestamp, reacto_time_t now)
{
    const reacto_time_t max = (reacto_time_t)-1;
    const reacto_time_t stupid_big_number = (max/2) + 1;
    reacto_time_t before = timestamp - stupid_big_number;
    return timeout_check_elapsed(now, before, stupid_big_number);
}

bool timeout_check(timeout_t * cobj, reacto_time_t tout)
{
    return timeout_check_elapsed(time_now(), *cobj, tout);
}

bool timeout_check_and_reinit(timeout_t * cobj, reacto_time_t period)
{
    if (!timeout_check_elapsed(time_now(), *cobj, period))
    {
    	return false;
    }
    else
    {
    	*cobj += period;
    	return true;
    }
}
