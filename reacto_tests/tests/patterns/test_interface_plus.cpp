#include <CppUTest/TestHarness.h>

extern "C"
{
    #include <patterns/interface.h>
}

TEST_GROUP(InterfacePlus)
{
    interface_t cut;

    void setup ()
    {
        interface_init(&cut, &interface_plus);
    }

    void teardown ()
    {
        interface_deinit(&cut);
    }
};

TEST(InterfacePlus, set_get)
{
    interface_set_number(&cut, 12371);
    int r = interface_get_number(&cut);
    CHECK_EQUAL(12371, r);
    interface_set_number(&cut, 200);
    r = interface_get_number(&cut);
    CHECK_EQUAL(12571, r);
}
