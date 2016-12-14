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
#ifndef CHECKS_H_
#define CHECKS_H_

#include "log.h"

/*
 * check_ptr
 * will check if `p` is NULL and will return the next argument.
 */
#define check_ptr(p, ...) do {if (!(p)) {log_error("Invalid Pointer"); return __VA_ARGS__;}} while(0)

/*
 * check_assert
 * will check if `p` solves to false and will return the next argument.
 */
#define check_assert(p, ...) do {if (!(p)) {log_error("Assertion failed: " #p); return __VA_ARGS__;}} while(0)


#endif /* CHECKS_H_ */
