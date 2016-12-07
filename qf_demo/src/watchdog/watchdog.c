#include <stddef.h>
#include <reusables/log.h>
#include <reusables/checks.h>

#include "hardware_watchdog.h"
#include "watchdog.h"
#include "hardware_watchdog.h"

static watchdog_t * root = NULL;

static void insert_entry (watchdog_t * obj)
{
    if (root)
        linked_list_insert_after(
                linked_list_last(root, list_head),
                obj,
                list_head);
    else
        root = obj;
}

static void remove_entry (watchdog_t * obj)
{
    root = linked_list_remove(obj, list_head);
}

static void check_initialize_hardware ()
{
    /* Only initialize hardware once */
    if (root)
        if (linked_list_next(root, list_head) == NULL)
            hardware_watchdog_init();
}

static void check_deinitialize_hardware ()
{
    /* Only deinitialize hardware once */
    if (!root)
        hardware_watchdog_deinit();
}

static bool is_active (watchdog_t * obj)
{
    return !!obj->timeout;
}

static bool is_expired (watchdog_t * obj)
{
    return timeout_check(&obj->timeout, obj->period_ms);
}

int watchdog_init(watchdog_t * obj, uint32_t period_ms, const char * name)
{
    check_ptr(obj, -1);

    obj->name = name;
    obj->period_ms = period_ms;
    obj->timeout = (timeout_t) 0;
    linked_list_init(obj, list_head);

    insert_entry(obj);
    check_initialize_hardware();

    return 0;
}

void watchdog_deinit (watchdog_t * obj)
{
    check_ptr(obj);

    watchdog_exit(obj);
    remove_entry (obj);
    check_deinitialize_hardware();
}

void watchdog_enter (watchdog_t * obj)
{
    check_ptr(obj);

    if (obj->timeout)
        return;

    timeout_init(&obj->timeout);
}

void watchdog_exit (watchdog_t * obj)
{
    check_ptr(obj);
    obj->timeout = (timeout_t) 0;
}

void watchdog_kick (watchdog_t * obj)
{
    check_ptr(obj);
    timeout_init(&obj->timeout);
}

void watchdog_periodic ()
{
    watchdog_t * entry = root;

    while (entry)
    {
        if (is_active(entry))
        {
            if (is_expired(entry))
            {
                log_message ("Watchdog %s expired.", entry->name);
                return;
            }
        }
        entry = linked_list_next(entry, list_head);
    }

    hardware_watchdog_kick();
}
