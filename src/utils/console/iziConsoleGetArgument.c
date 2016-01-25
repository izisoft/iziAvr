#include <izi/avr/utils/console.h>

extern char iziConsoleBuffer[IZI_CONSOLE_BUFFER_SIZE];

const char* iziConsoleGetArgument(uint8_t argNum)
{
	uint8_t carret = 0;

	while(argNum > 0)
	{
		while(iziConsoleBuffer[carret++] != '\0');
		if((carret == IZI_CONSOLE_BUFFER_SIZE - 1) || (iziConsoleBuffer[carret] == '\0'))
		{
			return NULL;
		}
		else
		{
			argNum--;
		}
	}

	return iziConsoleBuffer + carret;
}
