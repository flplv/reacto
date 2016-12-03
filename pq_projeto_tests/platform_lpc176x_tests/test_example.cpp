#include <CppUTest/TestHarness.h>

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
    }
};

TEST(FirstTestGroup, FirstTest)
{
   FAIL("failing!");
}

TEST(FirstTestGroup, LeakMeNicely)
{
   malloc(10);
}

TEST(FirstTestGroup, LeakMeNicely2)
{
   int * x = new int;
}

