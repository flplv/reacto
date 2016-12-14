#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

extern "C"
{
    #include <reusables/log.h>
}

TEST_GROUP(Log)
{
    void setup ()
    {
    }

    void teardown ()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(Log, periodic)
{
    mock().expectOneCall("_log_file_line")
            .withParameter("msg", "Error: oops!")
            .withParameter("file", __FILE__)
            .withParameter("line", __LINE__ + 2);
    /* Do not remove this line, nor add any extra line before log_error */
    log_error("oops!");
}
