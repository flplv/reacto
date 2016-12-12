#include "signal_slot_eq.h"

/* Setup macros to the template */
#define signal_prefix signal_eq_
#define slot_prefix slot_eq_
#define parameters_declaration(...) __VA_ARGS__,event_queue_t * the_queue
#define parameters_list(...) __VA_ARGS__, the_queue
#define handler_parameters_list(...) the_queue

/* Now we include the template, that was lots of fun, right? */
#include <reusables/signal_slot.template>
