#include <reusables/log.h>
#include <reusables/checks.h>

#include "abstract_data_type.h"

int adt_init(adt_t * obj)
{
    check_ptr(obj, -1);
    /* Perform initialization of obj */
    obj->internal_data = 0;
    return 0;
}

void adt_deinit(adt_t * obj)
{
    check_ptr(obj);
    /* Perform deinitialization of obj */
}

int adt_do_something(adt_t * obj, int parameter)
{
    check_ptr(obj, -1);
    obj->internal_data = parameter;
    return 0;
}
