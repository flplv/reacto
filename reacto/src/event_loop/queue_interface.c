#include "event_loop_types.h"
#include <reusables/checks.h>

bool queue_interface_emit(queue_i * itf)
{
    check_ptr(itf, false);
    check_ptr(itf->emitter, false);
    return itf->emitter(itf);
}

size_t queue_interface_count(queue_i * itf)
{
    check_ptr(itf, false);
    check_ptr(itf->count, false);
    return itf->count(itf);
}

size_t queue_interface_hash(queue_i * itf)
{
    check_ptr(itf, false);
    check_ptr(itf->count, false);
    return itf->hash(itf);
}

void queue_interface_pop(queue_i * itf)
{
    check_ptr(itf);
    check_ptr(itf->pop);
    itf->pop(itf);
}
