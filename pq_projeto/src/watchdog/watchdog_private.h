#ifndef WATCHDOG_PRIVATE_H_
#define WATCHDOG_PRIVATE_H_

#include <stdbool.h>
#include <reusables/linked_list.h>
#include <reusables/timeout.h>

struct _watchdog_private
{
    const char * name;
    uint32_t period_ms;
    timeout_t timeout;
    linked_list_t list_head;
};

#endif /* WATCHDOG_PRIVATE_H_ */
