#ifndef PQ_PROJETO_TESTS_PLATFORM_LPC176X_TESTS_PLAT_TIME_H_
#define PQ_PROJETO_TESTS_PLATFORM_LPC176X_TESTS_PLAT_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint32_t __time_now_ms;

static inline void time_tick ()
{
	__time_now_ms++;
}

#ifdef __cplusplus
}
#endif


#endif /* PQ_PROJETO_TESTS_PLATFORM_LPC176X_TESTS_PLAT_TIME_H_ */
