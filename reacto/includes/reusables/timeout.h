#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t timeout_t;

void timeout_init(timeout_t *);
timeout_t timeout_init_cpy(void);

/* Return true if tout_ms has elapsed */
bool timeout_check(timeout_t *, uint32_t tout_ms);

/* Return true if tout_ms has elapsed and reinit cobj, so it can be used after to create periodically execution */
bool timeout_check_and_reinit(timeout_t * cobj, uint32_t period_ms);

/* Sleep until the next timeout (use only to spend time while waiting for a timeout,
   do not count on time precision of this function) */
void timeout_sleep(timeout_t *cobj, uint32_t period_ms);

/* This is a elapsed time checker that protects against wrap around.
   Now must always be (physically) bigger than before.*/
bool timeout_check_elapsed(uint32_t now, uint32_t before, uint32_t desired_wait);

/* This is a reached time checker that protects against wrap around.
   It checks if now has reached timestamp. Timestamp must always be bigger than
   now when first stored. */
bool timeout_check_reached(uint32_t timestamp, uint32_t now);

#endif /* TIMEOUT_H_ */
