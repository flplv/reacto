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
#ifndef REACTO_REUSABLES_LOG_H_
#define REACTO_REUSABLES_LOG_H_

#ifndef REACTO_DISABLE_LOGGING

    __attribute__ ((format (printf, 3, 4))) void _log_file_line (const char * file, int line, const char * msg, ...);
    __attribute__ ((format (printf, 1, 2))) void log_message (const char * msg, ...);

    #ifdef REACTO_SHORT_LOGS
        #define log_error(...) log_message("[E]" __VA_ARGS__)
        #define log_warning(...) log_message("[W]" __VA_ARGS__)
    #else /* REACTO_SHORT_LOGS */
        #define log_error(...) _log_file_line(__FILE__, __LINE__, "Error: " __VA_ARGS__)
        #define log_warning(...) _log_file_line(__FILE__, __LINE__, "Warning: " __VA_ARGS__)
    #endif /* REACTO_SHORT_LOGS */

#else /* REACTO_DISABLE_LOGGING */

    __attribute__ ((format (printf, 1, 2))) static inline void log_error(const char * msg, ...) {msg = msg+1;};
    __attribute__ ((format (printf, 1, 2))) static inline void log_warning(const char * msg, ...) {msg = msg+1;};
    __attribute__ ((format (printf, 1, 2))) static inline void log_message(const char * msg, ...) {msg = msg+1;};

#endif /* REACTO_DISABLE_LOGGING */

#endif /* REACTO_REUSABLES_LOG_H_ */
