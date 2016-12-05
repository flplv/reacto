#include <stdio.h>
#include <CppUTest/CommandLineTestRunner.h>
#include <chip.h>
#include <board.h>


int main()
{
	printf ("Initializing Board\n");
	Chip_SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);

	Board_Init();
	Board_LED_Set(0, true);

	printf ("Platform tests with CppUTest\n");

	const char * args[] = {"./fw", "-v" };
    return CommandLineTestRunner::RunAllTests(2, args);
}
