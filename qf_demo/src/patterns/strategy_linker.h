#ifndef STRATEGY_LINKER_H
#define STRATEGY_LINKER_H

/*
 * Strategy Linker Pattern
 *
 * Intent: Define a family of algorithms, encapsulate each one and make them
 * interchangeable by the Linker.
 *
 * When should be used:
 *  To abstract platform differences, i.e. to adapt the build to different
 *   hardware configurations.
 *
 * How it works:
 *  It can have or not an object (ADT or Static Class);
 *  A header provides the available abstractions (functions);
 *  A C code that implement these functions is selected by the Linker
 *   at build time.
 *  Client depends only on the abstraction (header).
 */

int fi_init();
void fi_deinit();

void fi_do_something();
/* More get/set and methods for the Class here */

#endif
