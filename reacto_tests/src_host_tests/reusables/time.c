#include <reacto/reusables/time.h>

reacto_time_t time_now_variable = 0;

reacto_time_t time_now()
{
    return time_now_variable;
}
