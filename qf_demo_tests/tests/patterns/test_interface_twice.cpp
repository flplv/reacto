#include <CppUTest/TestHarness.h>

extern "C"
{
    #include <patterns/interface.h>
}

TEST_GROUP(InterfaceTwice)
{
    interface_t cut;

    void setup ()
    {
        interface_init(&cut, &interface_twice);
    }

    void teardown ()
    {
        interface_deinit(&cut);
    }
};

TEST(InterfaceTwice, set_get)
{
    interface_set_number(&cut, 12371);
    int r = interface_get_number(&cut);
    CHECK_EQUAL(24742, r);
}
