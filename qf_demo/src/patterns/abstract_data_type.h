#ifndef ABSTRACT_DATA_TYPE_H
#define ABSTRACT_DATA_TYPE_H

/*
 * Abstract Data Type (ADT) Pattern
 *
 * Objective: To hide from client code implementation symbols
 *
 * Advantages: Promotes decoupling between Classes.
 *
 * When should be used: Every time you need an instantiable object.
 *
 * Problems: Because there's no private field in C, ADT's
 *  struct internal data is in the scope of user code, it's up
 *  to him to not access fields directly. The header suffix
 * `_private.h` and the `private_` prefix in the struct name
 *  are there to explicit to the user this requirement.
 *
 * How it works:
 *  Every function takes the object as parameter;
 *  Init and Deinit are the Constructor and Destructor;
 *  No internal field can be accessed directly by client code;
 */

#include <patterns/abstract_data_type_private.h>

typedef struct private_adt adt_t;

int adt_init(adt_t * obj);
void adt_dinit(adt_t * obj);

int adt_do_something(adt_t * obj, int parameter);
/* More get/set and methods for the Class here */

#endif
