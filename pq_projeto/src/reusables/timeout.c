/*
 *    Copyright (c) 2013 Felipe Lavratti (felipe@andradeneves.com)
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *    THE SOFTWARE.
 */

#include <stdbool.h>
#include <reusables/timeout.h>
#include <reusables/time.h>

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


bool timeout_check(timeout_t * cobj, uint32_t tout_ms)
{
    return ( !( (time_now_ms() - *cobj) < tout_ms) );
}

bool timeout_check_and_reinit(timeout_t * cobj, uint32_t period_ms)
{
    if ( (time_now_ms() - *cobj) < period_ms )
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
