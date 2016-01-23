#include <stdlib.h>

#include <utils/iziConsole.h>

int iziConsoleGetArgumentAsInt(uint8_t argNum)
{
	const char* arg =  iziConsoleGetArgument(argNum);
	return (arg == NULL) ? 0 : atoi(arg);
}
