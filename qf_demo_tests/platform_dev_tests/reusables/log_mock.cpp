#include <CppUTestExt/MockSupport.h>

extern "C"
void log_write (const char * msg, const char * file, int line)
{
    /*
     * We want to use CppUTest Mock system in this function,
     * this way tests will fail if any code under tests print
     * an error message.
     */
    mock().actualCall("log_write")
            .withStringParameter("msg", msg)
            .withStringParameter("file", file)
            .withIntParameter("line", line);
}


extern "C"
void log_write2 (const char * msg)
{
    /*
     * We want to use CppUTest Mock system in this function,
     * this way tests will fail if any code under tests print
     * an error message.
     */
    mock().actualCall("log_write2")
            .withStringParameter("msg", msg);
}
