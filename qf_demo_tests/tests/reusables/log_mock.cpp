#include <CppUTestExt/MockSupport.h>

extern "C"
void _log_impl_file_line (const char * msg, const char * file, int line)
{
    /*
     * We want to use CppUTest Mock system in this function,
     * this way tests will fail if any code under tests print
     * an error message.
     */
    mock().actualCall("_log_impl_file_line")
            .withStringParameter("msg", msg)
            .withStringParameter("file", file)
            .withIntParameter("line", line);
}


extern "C"
void _log_impl (const char * msg)
{
    /*
     * We want to use CppUTest Mock system in this function,
     * this way tests will fail if any code under tests print
     * an error message.
     */
    mock().actualCall("_log_impl")
            .withStringParameter("msg", msg);
}
