#include <CppUTest/TestHarness.h>

extern "C"
{
    #include <patterns/strategy.h>
}

TEST_GROUP(StrategyPlus)
{
    strategy_t cut;

    void setup ()
    {
        strategy_init(&cut, &strategy_plus);
    }

    void teardown ()
    {
        strategy_deinit(&cut);
    }
};

TEST(StrategyPlus, set_get)
{
    strategy_set_number(&cut, 12371);
    int r = strategy_get_number(&cut);
    CHECK_EQUAL(12371, r);
    strategy_set_number(&cut, 200);
    r = strategy_get_number(&cut);
    CHECK_EQUAL(12571, r);
}
