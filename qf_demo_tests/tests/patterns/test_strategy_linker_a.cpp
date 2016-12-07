#include <CppUTest/TestHarness.h>

/*
 * Since there is more than one implementation that implements
 * the same symbols, we cannot link those source objects together.
 * But we can include `.c` directly, avoiding conflicts using C++
 * namespaces.
 */

namespace IMPL_A
{
    #include <patterns/strategy_linker_a.c>
}
using namespace IMPL_A;

TEST_GROUP(FlexibleImplementationImplA)
{
    void setup ()
    {
       fi_init();
    }

    void teardown ()
    {
        fi_deinit();
    }
};

TEST(FlexibleImplementationImplA, FirstTest)
{
   CHECK_TRUE(1 == 1);
}
