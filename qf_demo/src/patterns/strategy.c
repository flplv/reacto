#include <reusables/checks.h>
#include "strategy.h"

int strategy_init(strategy_t * obj, const struct strategy_behavior * behavior)
{
    check_ptr(obj, -1);
    obj->behavior = behavior;

    if (obj->behavior->init)
        return obj->behavior->init(obj);

    return 0;
}

void strategy_deinit(strategy_t * obj)
{
    check_ptr(obj);

    if (obj->behavior->deinit)
        obj->behavior->deinit(obj);
}

void strategy_set_number(strategy_t * obj, int number)
{
    check_ptr(obj);

    if (obj->behavior->set_number)
        obj->behavior->set_number(obj, number);
    else
        obj->number = number;
}

int strategy_get_number(strategy_t * obj)
{
    check_ptr(obj, 0);

    if (obj->behavior->get_number)
        return obj->behavior->get_number(obj);

    return obj->number;
}

/* Fall back to default behavior */
const struct strategy_behavior strategy_normal = { 0,};
