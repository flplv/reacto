#include <CppUTest/TestHarness.h>

extern "C"
{
    #include <patterns/strategy.h>
}

TEST_GROUP(StrategyNormal)
{
    strategy_t cut;

    void setup ()
    {
        strategy_init(&cut, &strategy_normal);
    }

    void teardown ()
    {
        strategy_deinit(&cut);
    }
};

TEST(StrategyNormal, set_get)
{
    strategy_set_number(&cut, 12371);
    int r = strategy_get_number(&cut);
    CHECK_EQUAL(12371, r);
}
