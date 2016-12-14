#include <reusables/checks.h>
#include <reusables/macros.h>
#include <reusables/log.h>
#include <reusables/time.h>
#include <reusables/timeout.h>
#include <timed_queue.h>
#include <main_loop.h>
#include <signal_slot_queue.h>

static bool emitter(queue_i * itf)
{
    timed_queue_t * obj = container_of(itf, typeof(*obj), itf);
    timed_event_t * ev = obj->root;

    if (!timeout_check_reached(ev->timestamp, time_now_ms()))
        return true; /* Skip this queue */

    obj->root = linked_list_remove(ev, ll);
    obj->cnt_cache--;
    ev->handler(ev);
    return false;
}

static size_t count(queue_i * itf)
{
    timed_queue_t * obj = container_of(itf, typeof(*obj), itf);
    return obj->cnt_cache;
}

static size_t hash(queue_i * obj)
{
    obj = obj + 1;
    return 0;
}

static void pop(queue_i * obj)
{
    obj = obj + 1;
}

void timed_queue_init(timed_queue_t * obj)
{
    check_ptr(obj);

    obj->itf.loop = NULL;
    obj->itf.emitter = emitter;
    obj->itf.pop = pop;
    obj->itf.count = count;
    obj->itf.hash = hash;
    linked_list_init(&obj->itf, ll);
    obj->root = NULL;
    obj->cnt_cache = 0;
}

static void _free (timed_event_t * ev)
{
    linked_list_init(ev, ll);
}

void timed_queue_deinit(timed_queue_t * obj)
{
    check_ptr(obj);

    if (obj->itf.loop)
        main_loop_remove_queue(obj->itf.loop, &obj->itf);

    obj->itf.loop = 0;
    linked_list_free(obj->root, ll, _free);
    obj->root = NULL;
    obj->cnt_cache = 0;
}

void timed_event_init(timed_event_t * ev, uint32_t timeout, timed_event_handler_t handler)
{
    check_ptr(ev);
    check_ptr(handler);
    ev->timestamp = time_now_ms() + timeout;
    ev->handler = handler;
    linked_list_init(ev, ll);
}

static bool comp (uint32_t seed, timed_event_t * ev)
{
    return ev->timestamp > seed ? true : false;
}

void timed_queue_link(timed_queue_t * obj, timed_event_t * ev)
{
    check_ptr(obj);
    check_ptr(ev);

    linked_list_init(ev, ll);

    if (!obj->root)
    {
        obj->root = ev;
    }
    else
    {
        timed_event_t * after = linked_list_find(obj->root, ll, comp, ev->timestamp);

        if (after)
        {
            linked_list_insert_before(after, ev, ll);
            obj->root = linked_list_first(ev, ll);
        }
        else
        {
            linked_list_insert_after(linked_list_last(obj->root, ll), ev, ll);
        }
    }

    obj->cnt_cache++;
}

queue_i * timed_queue_interface (timed_queue_t * obj)
{
    check_ptr(obj, NULL);
    return &obj->itf;
}
