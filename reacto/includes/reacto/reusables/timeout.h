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
#ifndef REACTO_REUSABLES_TIMEOUT_H_
#define REACTO_REUSABLES_TIMEOUT_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t timeout_t;

void timeout_init(timeout_t *);
void timeout_copy(timeout_t * to, timeout_t * from);

/* Return true if tout has elapsed */
bool timeout_check(timeout_t *, uint32_t tout);

/* Return true if tout has elapsed and reinit cobj, so it can be used after to create periodically execution */
bool timeout_check_and_reinit(timeout_t * cobj, uint32_t period);

/* Sleep until the next timeout (use only to spend time while waiting for a timeout,
   do not count on time precision of this function) */
void timeout_sleep(timeout_t *cobj, uint32_t period);

/* This is a elapsed time checker that protects against wrap around.
   Now must always be (physically) bigger than before.*/
bool timeout_check_elapsed(uint32_t now, uint32_t before, uint32_t desired_wait);

/* This is a reached time checker that protects against wrap around.
   It checks if now has reached timestamp. Timestamp must always be bigger than
   now when first stored. */
bool timeout_check_reached(uint32_t timestamp, uint32_t now);

#endif /* REACTO_REUSABLES_TIMEOUT_H_ */
