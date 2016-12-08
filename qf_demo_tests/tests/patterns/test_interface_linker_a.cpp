#include <CppUTest/TestHarness.h>

/*
 * Since there is more than one implementation that implements
 * the same symbols, we cannot link those source objects together.
 * But we can include `.c` directly, avoiding conflicts using C++
 * namespaces.
 */

namespace IMPL_A
{
    #include <patterns/interface_linker_a.c>
}
using namespace IMPL_A;

TEST_GROUP(InterfaceLinkerA)
{
    void setup ()
    {
       interface_linker_init();
    }

    void teardown ()
    {
        interface_linker_deinit();
    }
};

TEST(InterfaceLinkerA, FirstTest)
{
    interface_linker_do_something();
    CHECK_TRUE(1 == 1);
}
