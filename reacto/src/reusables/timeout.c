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
    *cobj = time_now_ms();
}

timeout_t timeout_init_cpy(void)
{
	timeout_t cobj;

	timeout_init(&cobj);

	return cobj;
}

void timeout_copy(timeout_t * to, timeout_t * from)
{
	*to = *from;
}

bool timeout_check_elapsed(uint32_t now, uint32_t before, uint32_t desired_wait)
{
    return (now - before) >= desired_wait;
}

bool timeout_check_reached(uint32_t timestamp, uint32_t now)
{
    const uint32_t stupid_big_number = 0x80000000;
    uint32_t before = timestamp - stupid_big_number;
    return timeout_check_elapsed(now, before, stupid_big_number);
}

bool timeout_check(timeout_t * cobj, uint32_t tout_ms)
{
    return timeout_check_elapsed(time_now_ms(), *cobj, tout_ms);
}

bool timeout_check_and_reinit(timeout_t * cobj, uint32_t period_ms)
{
    if (!timeout_check_elapsed(time_now_ms(), *cobj, period_ms))
    {
    	return false;
    }
    else
    {
    	*cobj += period_ms;
    	return true;
    }
}

void timeout_sleep(timeout_t *cobj, uint32_t period_ms)
{
	if (timeout_check(cobj, period_ms))
		return;

	time_sleep((*cobj + period_ms) - time_now_ms());
}
