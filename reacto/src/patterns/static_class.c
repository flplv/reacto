#include <reusables/log.h>
#include <reusables/checks.h>

#include "static_class.h"

struct private_static {
    int internal_data;
};

static struct private_static static_data;

int sc_init()
{
    /* Perform initialization of the static object */
    static_data.internal_data = 0;
    return 0;
}

void sc_deinit()
{
    /* Perform deinitialization of static object */
}

void sc_set_something(int parameter)
{
    static_data.internal_data = parameter;
}
