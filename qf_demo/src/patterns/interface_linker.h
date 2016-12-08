#ifndef INTERFACE_LINKER_H
#define INTERFACE_LINKER_H

/*
 * Interface Linker Pattern
 *
 * Intent: Define a contract where every code involved will depend on.
 *
 * Also know as:
 *
 *
 * When should be used:
 *  Every time you have 1:1 configurable dependency: a 1 client must use 1
 *   object with different implementations that can be selected on build time.
 *  Menu navigation, event abstraction, etc.
 *
 * When should be used:
 *  To abstract platform differences, i.e. to adapt the build to different
 *   hardware configurations.
 *
 * How it works:
 *  It can be a Class or a Static Class;
 *  A header provides the available abstractions (functions);
 *  A C code that implement these functions is selected by the Linker
 *   at build time.
 *  Client depends only on the abstraction (header).
 */

int interface_linker_init();
void interface_linker_deinit();

void interface_linker_do_something();
/* More get/set and methods for the Class here */

#endif
