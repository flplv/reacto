#include "strategy.h"

static int init(strategy_t * obj)
{
    return 0;
}

static void deinit(strategy_t * obj)
{
}

static int get_number(strategy_t * obj)
{
    return obj->number;
}

static void set_number(strategy_t * obj, int number)
{
    obj->number = obj->number + number;
}

const struct strategy_behavior strategy_plus =
{
        .init = init,
        .deinit = deinit,
        .get_number = get_number,
        .set_number = set_number,
};
