#ifndef FLEXIBLE_IMPLEMENTATION_H
#define FLEXIBLE_IMPLEMENTATION_H

/*
 * Flexible Implementation Pattern
 * It is a variation of the Abstract Data Type or the Static
 *  Class where you can change implementation using the Linker.
 *
 * Objective: To select implementation at the link time.
 *
 * Advantages:
 *  Permits changes of the behaviour;
 *
 * When sould be used:
 *  To abstract platform differences, i.e. to adapt the build
 *  to different hardware configurations.
 *
 * How it works:
 *  It can have or not an object (ADT or Static Class);
 *  A header provides the available functions;
 *  A C code that implement these functions is select by the Linker
 *   at build time.
 */


int fi_init();
void fi_deinit();

void fi_do_something();
/* More get/set and methods for the Class here */

#endif
