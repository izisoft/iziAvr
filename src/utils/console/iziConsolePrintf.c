#include <stdio.h>

#include <izi/avr/utils/console.h>
#include <izi/avr/driver/uart.h>
#include <izi/avr/core/memory.h>

void iziConsolePrintf(const char* fmt,...)
{
	uint8_t dataCount;
	char *buffer = iziMalloc(IZI_CONSOLE_BUFFER_SIZE);

	va_list ap;
	va_start(ap,fmt);

	dataCount = vsnprintf(buffer,IZI_CONSOLE_BUFFER_SIZE,fmt,ap);
	if(dataCount > IZI_CONSOLE_BUFFER_SIZE)
	{
		dataCount = IZI_CONSOLE_BUFFER_SIZE;
	}
	iziUartWriteStream(eIziUartInterface_0,(const char*)buffer,dataCount);

	iziFree((void*)buffer);
}
