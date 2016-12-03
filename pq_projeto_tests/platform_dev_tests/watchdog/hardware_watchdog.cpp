#include <CppUTestExt/MockSupport.h>

extern "C" void hardware_watchdog_init()
{
    mock().actualCall("hardware_watchdog_init");
}

extern "C" void hardware_watchdog_kick()
{
    mock().actualCall("hardware_watchdog_kick");
}

extern "C" void hardware_watchdog_deinit()
{
    mock().actualCall("hardware_watchdog_deinit");
}
