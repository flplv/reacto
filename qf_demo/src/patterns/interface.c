#include <reusables/checks.h>
#include "interface.h"

int interface_init(interface_t * obj, const struct interface_behavior * behavior)
{
    check_ptr(obj, -1);
    check_ptr(behavior, -1);
    obj->behavior = behavior;

    check_ptr(obj->behavior->init, -1);
    return obj->behavior->init(obj);
}

void interface_deinit(interface_t * obj)
{
    check_ptr(obj);
    check_ptr(obj->behavior);
    check_ptr(obj->behavior->deinit);
    obj->behavior->deinit(obj);
}

void interface_set_number(interface_t * obj, int number)
{
    check_ptr(obj);
    check_ptr(obj->behavior);
    check_ptr(obj->behavior->set_number);
    obj->behavior->set_number(obj, number);
}

int interface_get_number(interface_t * obj)
{
    check_ptr(obj, 0);
    check_ptr(obj->behavior, 0);
    check_ptr(obj->behavior->get_number, 0);
    return obj->behavior->get_number(obj);
}
