#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <izi/avr/core/kernel.h>
#include <izi/avr/core/memory.h>
#include <izi/avr/utils/logger.h>
#include <izi/avr/driver/uart.h>

void iziLoggerInit(uint32_t baudrate)
{
	IZI_ASSERT(!iziKernelCheckState(eIziLoggerInit));
	IZI_ASSERT(!iziKernelCheckState(eIziConsoleInit));

	iziUartConfigureUnit(eIziUartInterface_0,baudrate);
	iziKernelSetState(eIziLoggerInit);
}

void iziLog(const char* fmt,...)
{
	uint8_t dataCount;
	char *buffer = iziMalloc(IZI_LOGGER_BUFFER_SIZE);

	va_list ap;
	va_start(ap,fmt);

	dataCount = vsnprintf(buffer,IZI_LOGGER_BUFFER_SIZE,fmt,ap);
	if(dataCount > IZI_LOGGER_BUFFER_SIZE)
	{
		dataCount = IZI_LOGGER_BUFFER_SIZE;
	}
	iziUartWriteStream(eIziUartInterface_0,(const char*)buffer,dataCount);

	iziFree((void*)buffer);
}
