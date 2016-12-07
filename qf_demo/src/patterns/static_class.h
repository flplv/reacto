#ifndef STATIC_CLASS_H
#define STATIC_CLASS_H

/*
 * Static Class Pattern
 * It is a variation of the Abstract Data Type, where you cannot
 *  create an instance of the object.
 *
 * Objective: To abstract static data and implementation.
 *
 * Advantages:
 *  Promotes decoupling between Classes;
 *  Hides internal implementation from client code.
 *
 * When sould be used: Everytime you need an static behaviour.
 *
 */

int sc_init();
void sc_deinit();

void sc_set_something(int parameter);
/* More set/get and methods for the Class here */

#endif
