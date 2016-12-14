#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*
 * Basic testing guideline:
 * 1) Test all functionalities
 * 2) Test buffer boundaries
 * 3) Test invalid arguments
 */


TEST_GROUP(FirstTestGroup)
{
    void setup ()
    {
    }

    void teardown ()
    {
        mock().clear();
    }
};

TEST(FirstTestGroup, FirstTest)
{
   int x = 1;
   CHECK_EQUAL(1, x);
}

