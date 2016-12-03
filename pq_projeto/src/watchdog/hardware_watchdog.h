#ifndef PLATFORM_WATCHDOG_H_
#define PLATFORM_WATCHDOG_H_

void hardware_watchdog_init();
void hardware_watchdog_kick();
void hardware_watchdog_deinit();

#endif /* PLATFORM_WATCHDOG_H_ */
