#include <CppUTest/TestHarness.h>

/*
 * Since there is more than one implementation that implements
 * the same symbols, we cannot link those source objects together.
 * But we can include `.c` directly, avoiding conflicts using C++
 * namespaces.
 */

namespace IMPL_B
{
    #include <patterns/flexible_implementation_impl_b.c>
}
using namespace IMPL_B;


TEST_GROUP(FlexibleImplementationImplB)
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

TEST(FlexibleImplementationImplB, FirstTest)
{
   CHECK_TRUE(1 == 1);
}
