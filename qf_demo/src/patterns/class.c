#include <reusables/log.h>
#include <reusables/checks.h>

#include "class.h"

int class_init(class_t * obj)
{
    check_ptr(obj, -1);
    /* Perform initialization of obj */
    obj->internal_data = 0;
    return 0;
}

void class_deinit(class_t * obj)
{
    check_ptr(obj);
    /* Perform deinitialization of obj */
}

int class_do_something(class_t * obj, int parameter)
{
    check_ptr(obj, -1);
    obj->internal_data = parameter;
    return 0;
}
