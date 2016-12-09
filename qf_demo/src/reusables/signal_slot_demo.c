#include "signal_slot_demo.h"

/* Setup macros to the template */
#define signal_prefix signal_demo_
#define slot_prefix slot_demo_
#define parameters_declaration(...) __VA_ARGS__,int a_number,char * a_string
#define parameters_list(...) __VA_ARGS__,a_number,a_string

/* Now we include the template, that was lots of fun, right? */
#include "signal_slot.template"
