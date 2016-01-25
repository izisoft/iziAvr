#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <izi/avr/core/memory.h>
#include <izi/avr/core/task.h>
#include <izi/avr/driver/uart.h>
#include <izi/avr/utils/console.h>

#include <core/kernel/iziKernelPriv.h>
#include <device/iziDevicePriv.h>

#include "iziConsoleCommon.h"

char iziConsoleBuffer[IZI_CONSOLE_BUFFER_SIZE];
char iziConsoleLastCommand[IZI_CONSOLE_BUFFER_SIZE];
TIziConsoleCommand* iziConsoleCommandList = NULL;

void iziConsoleTask()
{
	char c = '\0';
	uint8_t carret = 0;
	TIziConsoleCommand *cmd;
	
	iziConsolePrintf_P(IZI_PSTR("\fiziConsole started\n\r"));

	memset((void*)iziConsoleLastCommand,0x00,IZI_CONSOLE_BUFFER_SIZE);
	for(;;) {
		memset((uint8_t*)iziConsoleBuffer,0x00,IZI_CONSOLE_BUFFER_SIZE);
		iziConsolePrintf_P(IZI_PSTR("izi> "));
		for(;;) {
			c = iziUartGetchar(eIziUartInterface_0);
			switch(c){
				case '\r':
					iziUartPutchar(eIziUartInterface_0,c);
					iziUartPutchar(eIziUartInterface_0,'\n');
					c = '\0';
					break;
				case '\b':
				case 127 :
					iziUartPutchar(eIziUartInterface_0,127);
					if(carret > 0) {
						carret--;
					} else {
						iziUartPutchar(eIziUartInterface_0,' ');
					}
					break;
				case  0x1B:
					iziUartGetchar(eIziUartInterface_0);
					c = iziUartGetchar(eIziUartInterface_0);
					if( c == 'A') {
						c = 0x1B;
					}
					break;
				default:
					if( (c >= 0x20) && (c < 0x7F) ) {
						iziUartPutchar(eIziUartInterface_0,c);
						iziConsoleBuffer[carret++] = c;
					}
					break;
			}
			if(carret >= IZI_CONSOLE_BUFFER_SIZE - 1) {
				iziUartPutchar(eIziUartInterface_0,'\n');
				iziUartPutchar(eIziUartInterface_0,'\r');
				c = '\0';
			}
			if ( c == 0x1B) {
				if(iziConsoleLastCommand[0] != '\0') {
					memcpy(iziConsoleBuffer,iziConsoleLastCommand,strlen(iziConsoleLastCommand)+1);
					while(carret-- > 0) {
						iziUartPutchar(eIziUartInterface_0,127);
					}
					for(;;) {
						if(iziConsoleBuffer[++carret] == '\0') {
							break;
						}
						iziUartPutchar(eIziUartInterface_0,iziConsoleBuffer[carret]);
					}
				}
			}
			if(c == '\0') {
				iziConsoleBuffer[carret] = c;
				break;
			}
		}
		memcpy(iziConsoleLastCommand,iziConsoleBuffer,strlen(iziConsoleBuffer)+1);
		for(carret=0; carret < IZI_CONSOLE_BUFFER_SIZE; carret++) {
			if(iziConsoleBuffer[carret] == ' ') {
				iziConsoleBuffer[carret] = '\0';
			}
		}
		carret=0;
		cmd = iziConsoleCommandList;
		while(cmd != NULL){
			if(strcmp(cmd->cmd,iziConsoleBuffer) == 0) {
				cmd->callback();
				break;
			} 
			cmd = cmd->next;
		}
		if (cmd == NULL) {
			iziConsolePrintf_P(IZI_PSTR("Unknown command\n\r"));
		}
	}
}
void iziConsolePrintf_P(const char* fmt, ...)
{
	uint8_t dataCount;
	char *buffer = iziMalloc(IZI_CONSOLE_BUFFER_SIZE);

	va_list ap;
	va_start(ap,fmt);

	//TODO prepare multiplatform version
	dataCount = vsnprintf_P(buffer,IZI_CONSOLE_BUFFER_SIZE,fmt,ap);
	if(dataCount > IZI_CONSOLE_BUFFER_SIZE) {
		dataCount = IZI_CONSOLE_BUFFER_SIZE;
	}
	iziUartWriteStream(eIziUartInterface_0,(const char*)buffer,dataCount);

	iziFree((void*)buffer);
}

TIziTask *iziConsoleInit(uint32_t baudrate, uint16_t stackSize)
{
	IZI_ASSERT(!IZI_GET_STATE(eIziLoggerInit));
	IZI_ASSERT(!IZI_GET_STATE(eIziConsoleInit));
	iziUartConfigureUnit(eIziUartInterface_0,baudrate);
	IZI_SET_STATE(eIziConsoleInit);
	return iziTaskCreate(iziConsoleTask,eIziPrioLow,stackSize,NULL);
}
