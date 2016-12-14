#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>

/*
 * Get current platform time in milliseconds
 */
uint32_t time_now_ms(void);
void time_sleep (uint32_t ms);

#endif /* TIME_H_ */
