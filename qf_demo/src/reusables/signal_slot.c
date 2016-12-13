#include "signal_slot.h"

/* Setup macros to the template */
#define signal_prefix signal_
#define slot_prefix slot_
#define parameters_declaration(...) __VA_ARGS__
#define parameters_list(...) __VA_ARGS__
#define handler_parameters_list(...)  __VA_ARGS__

/* Now we include the template, that was lots of fun, right? */
#include "signal_slot.template"
