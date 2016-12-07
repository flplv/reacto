#include "strategy.h"

static void set_number(strategy_t * obj, int number)
{
    obj->number = number * 2;
}

const struct strategy_behavior strategy_twice =
{
    .set_number = set_number,
};
