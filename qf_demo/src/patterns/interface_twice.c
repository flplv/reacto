#include "interface.h"

static void set_number(interface_t * obj, int number)
{
    obj->number = number * 2;
}

const struct interface_behavior interface_twice =
{
    .set_number = set_number,
};
