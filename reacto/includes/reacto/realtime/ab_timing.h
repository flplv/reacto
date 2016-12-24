#ifndef REACTO_REALTIME_AB_TIMING_H_
#define REACTO_REALTIME_AB_TIMING_H_

#include <reacto/reusables/debug.h>
#include <reacto/reusables/timeout.h>

struct ab_timing_private
{
    timeout_t t;
};

typedef struct ab_timing_private ab_timing_t;

static inline void ab_timing_start(ab_timing_t * obj)
{
    debug_ptr(obj);
    timeout_init(&obj->t);
}

static inline void ab_timing_copy(ab_timing_t * to, ab_timing_t * from)
{
    debug_ptr(to);
    debug_ptr(from);
    timeout_copy(&to->t, &from->t);
}

static inline bool ab_timing_check_criteria_failed(ab_timing_t * obj, uint32_t time_criteria)
{
    debug_ptr(obj, false);
    return timeout_check(&obj->t, time_criteria);
}



#endif /* REACTO_REALTIME_AB_TIMING_H_ */
