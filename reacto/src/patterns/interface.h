#ifndef INTERFACE_H
#define INTERFACE_H

/*
 * Interface Pattern
 *
 * Intent: Define a contract where every code involved will depend on.
 *
 * Also know as:
 *
 *
 * When should be used:
 *  Every time you have 1:N dependency: a 1 client must use N objects with
 *   different implementations.
 *  Menu navigation, event abstraction, etc.
 *
 *
 * How it works:
 *  It must be a Class, to store selected behavior;
 *  Client code selects behavior at the `init`.
 *  The interface.c file redirect calls to implementations.
 *  Client depends only on this abstraction (header).
 *  The interface_private.h is there only to hide implementation details from
 *   the header file.
 */

#include "interface_private.h"

/*
 * Available behaviors
 */
/* Twice behavior will duplicate the number set */
extern const struct interface_behavior interface_twice;
/* Plus behavior will add the number set */
extern const struct interface_behavior interface_plus;


int interface_init(interface_t * obj, const struct interface_behavior * behavior);
void interface_deinit(interface_t * obj);

void interface_set_number(interface_t * obj, int number);
int interface_get_number(interface_t * obj);

#endif
