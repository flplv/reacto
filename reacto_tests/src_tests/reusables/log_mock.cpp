#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdarg.h>
#include <stdio.h>

/* Bypass cpputest memory leak detection */
#ifdef realloc
#undef realloc
#endif

static size_t get_buffer (size_t n, char ** const ret_buf)
{
    static char * buf = NULL;
    static size_t size = 0;

    if (n > size)
    {
        size = n * 2;
        buf = (char *)realloc(buf, size);
    }

    if (ret_buf)                                                //LCOV_EXCL_LINE
        *ret_buf = buf;
    return size;
}

static const char * format(const char * format, va_list ap)
{
    char * buf;

    va_list ap2;
    va_copy(ap2, ap);

    int n = vsnprintf(NULL, 0, format, ap);
    if (n < 0)                                                  //LCOV_EXCL_LINE
        FAIL("Error on vsnprintf");                             //LCOV_EXCL_LINE

    size_t size = get_buffer((size_t)n, &buf);

    vsnprintf(buf, size, format, ap2);
    va_end(ap2);
    return buf;
}

extern "C"
void _log_file_line (const char * file, int line, const char * msg, ...)
{
    /*
     * We want to use CppUTest Mock system in this function,
     * this way tests will fail if any code under tests print
     * an error message.
     */
    va_list ap;
    va_start(ap, msg);

    mock().actualCall("_log_file_line")
            .withStringParameter("msg", format(msg, ap))
            .withStringParameter("file", file)
            .withIntParameter("line", line);

    va_end(ap);
}


extern "C"
void log_message (const char * msg, ...)
{
    /*
     * We want to use CppUTest Mock system in this function,
     * this way tests will fail if any code under tests print
     * an error message.
     */
    va_list ap;
    va_start(ap, msg);

    mock().actualCall("log_message")
            .withStringParameter("msg", format(msg, ap));

    va_end(ap);
}
