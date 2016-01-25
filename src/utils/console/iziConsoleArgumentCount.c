#include <string.h>

#include <izi/avr/utils/console.h>

extern char iziConsoleBuffer[IZI_CONSOLE_BUFFER_SIZE];

uint8_t iziConsoleArgumentCount()
{
	uint8_t carret = 0;
	uint8_t argNum = 0;

	for(; carret < IZI_CONSOLE_BUFFER_SIZE - 1; carret++)
	{
		if(iziConsoleBuffer[carret] == '\0')
		{
			if(iziConsoleBuffer[carret + 1] != '\0')
			{
				argNum++;
			}
			else
			{
				return argNum;
			}
		}
	}
	return argNum;
}

