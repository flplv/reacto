#include "interface.h"

static int init(interface_t * obj)
{
    obj->number = 0;
    return 0;
}

static void deinit(interface_t * obj)
{
    obj->number = 0;
}

static int get_number(interface_t * obj)
{
    return obj->number;
}

static void set_number(interface_t * obj, int number)
{
    obj->number = obj->number + number;
}

const struct interface_behavior interface_plus =
{
        .init = init,
        .deinit = deinit,
        .get_number = get_number,
        .set_number = set_number,
};
