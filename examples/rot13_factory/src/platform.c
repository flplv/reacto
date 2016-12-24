#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <reacto/reusables/log.h>

void _log_file_line (const char * file, int line, const char * msg, ...)
{
    /*
     * We want to use CppUTest Mock system in this function,
     * this way tests will fail if any code under tests print
     * an error message.
     */
    va_list ap;
    va_start(ap, msg);

    printf("%s:%d ", file, line);
    vprintf(msg, ap);
    printf("\n");

    va_end(ap);
}


void log_message (const char * msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
}

uint32_t time_now()
{
    struct timeval tv;

    int r = gettimeofday(&tv, NULL);

    if (r != 0)                                                      //LCOV_EXCL_LINE
    {                                                                //LCOV_EXCL_LINE
        log_error("Unable to get time of day: %s", strerror(errno)); //LCOV_EXCL_LINE
        return 0;                                                    //LCOV_EXCL_LINE
    }                                                                //LCOV_EXCL_LINE

    uint32_t msec = (uint32_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
    return msec;
}

void time_sleep (uint32_t ms)
{
    usleep(ms * 1000);
}
