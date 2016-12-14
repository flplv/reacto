#include <CppUTest/TestHarness.h>

/*
 * Since there is more than one implementation that implements
 * the same symbols, we cannot link those source objects together.
 * But we can include `.c` directly, avoiding conflicts using C++
 * namespaces.
 */

namespace IMPL_B
{
    #include <patterns/interface_linker_b.c>
}
using namespace IMPL_B;


TEST_GROUP(InterfaceLinkerB)
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

TEST(InterfaceLinkerB, FirstTest)
{
    interface_linker_do_something();
    CHECK_TRUE(1 == 1);
}
