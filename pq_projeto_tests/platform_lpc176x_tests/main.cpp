#include <stdio.h>
#include <CppUTest/CommandLineTestRunner.h>

int main()
{
	printf ("Platform tests with CppUTest\n");

	const char * args[] = {"./fw", };
    return CommandLineTestRunner::RunAllTests(1, args);
}
