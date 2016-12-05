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
   CHECK_TRUE(1 == 1);
}

TEST(FirstTestGroup, LeakMeNicely)
{
   void * v = malloc(10);
   free(v);  /* Comment this line to fail on memory leak check */
}

TEST(FirstTestGroup, LeakMeNicely2)
{
   int * x = new int;
   delete x; /* Comment this line to fail on memory leak check */
}

