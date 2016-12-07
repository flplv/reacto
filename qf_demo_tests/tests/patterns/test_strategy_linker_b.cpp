#include <CppUTest/TestHarness.h>

/*
 * Since there is more than one implementation that implements
 * the same symbols, we cannot link those source objects together.
 * But we can include `.c` directly, avoiding conflicts using C++
 * namespaces.
 */

namespace IMPL_B
{
    #include <patterns/strategy_linker_b.c>
}
using namespace IMPL_B;


TEST_GROUP(StrategyLinkerB)
{
    void setup ()
    {
        strategy_linker_init();
    }

    void teardown ()
    {
        strategy_linker_deinit();
    }
};

TEST(StrategyLinkerB, FirstTest)
{
   CHECK_TRUE(1 == 1);
}
