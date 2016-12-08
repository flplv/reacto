#include <reusables/checks.h>
#include "interface.h"

int interface_init(interface_t * obj, const struct interface_behavior * behavior)
{
    check_ptr(obj, -1);
    obj->behavior = behavior;

    if (obj->behavior->init)
        return obj->behavior->init(obj);

    return 0;
}

void interface_deinit(interface_t * obj)
{
    check_ptr(obj);

    if (obj->behavior->deinit)
        obj->behavior->deinit(obj);
}

void interface_set_number(interface_t * obj, int number)
{
    check_ptr(obj);

    if (obj->behavior->set_number)
        obj->behavior->set_number(obj, number);
    else
        obj->number = number;
}

int interface_get_number(interface_t * obj)
{
    check_ptr(obj, 0);

    if (obj->behavior->get_number)
        return obj->behavior->get_number(obj);

    return obj->number;
}

/* Fall back to default behavior */
const struct interface_behavior interface_normal = { 0,};
