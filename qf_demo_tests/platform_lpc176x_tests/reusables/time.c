#include <reusables/time.h>

uint32_t __time_now_ms = 0;

uint32_t time_now_ms()
{
	return __time_now_ms;
}

void time_sleep (uint32_t ms)
{
	uint32_t start = __time_now_ms;
    while ((__time_now_ms - start) < ms);
}
