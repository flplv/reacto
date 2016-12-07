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

TEST_GROUP(StrategyLinkerA)
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

TEST(StrategyLinkerA, FirstTest)
{
    strategy_linker_do_something();
    CHECK_TRUE(1 == 1);
}
