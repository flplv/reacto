#ifndef CHECKS_H_
#define CHECKS_H_

#include "log.h"

/*
 * check_ptr
 * will check if `p` is NULL and will return the next argument.
 */
#define check_ptr(p, ...) do {if (!(p)) {log_error("Invalid Pointer"); return __VA_ARGS__;}} while(0)

/*
 * check_assert
 * will check if `p` solves to false and will return the next argument.
 */
#define check_assert(p, ...) do {if (!(p)) {log_error("Assertion failed: " #p); return __VA_ARGS__;}} while(0)


#endif /* CHECKS_H_ */
