#ifndef CLASS_H
#define CLASS_H

/*
 * Class Pattern
 *
 * Intent: A multiple instance container that have one single objective,
 *  to avoid coupling between implementation and client code and to be unit
 *  testable.
 *
 * When should be used: Every time you need to implement a single functionality,
 *  all elements of the firmware have to be one type of Class.
 *
 * How it works:
 *  Every function takes the object as parameter;
 *  Init and Deinit are the Constructor and Destructor;
 *  Client code must not access any field of the object directly;
 *  A `_private.h` header is used to store the private data and a header shows
 *   the public methods.
 */

struct _class_private
{
    int internal_data;
};

typedef struct _class_private class_t;

int class_init(class_t * obj);
void class_dinit(class_t * obj);

int class_do_something(class_t * obj, int parameter);
/* More get/set and methods for the Class here */

#endif
