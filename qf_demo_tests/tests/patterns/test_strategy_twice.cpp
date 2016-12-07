#include <CppUTest/TestHarness.h>

extern "C"
{
    #include <patterns/strategy.h>
}

TEST_GROUP(StrategyTwice)
{
    strategy_t cut;

    void setup ()
    {
        strategy_init(&cut, &strategy_twice);
    }

    void teardown ()
    {
        strategy_deinit(&cut);
    }
};

TEST(StrategyTwice, set_get)
{
    strategy_set_number(&cut, 12371);
    int r = strategy_get_number(&cut);
    CHECK_EQUAL(24742, r);
}
