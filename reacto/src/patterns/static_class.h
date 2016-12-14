#ifndef STATIC_CLASS_H
#define STATIC_CLASS_H

/*
 * Static Class Pattern
 *
 * Intent: A non instantiable container that have one single objective, to avoid
 * coupling between implementation and client code and to be unit testable.
 *
 * When should be used: Every time you need to implement a single functionality
 *  that deals only with global resources..
 *
 * How it works:
 *  Init and Deinit are the Constructor and Destructor;
 *  Every public methods must be defined in the header.
 */


int sc_init();
void sc_deinit();

void sc_set_something(int parameter);
/* More set/get and methods for the Class here */

#endif
