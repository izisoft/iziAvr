#include <avr/io.h>
#include <avr/interrupt.h>

#include <iziAvr.h>
#include <core/iziKernel.h>
#include <core/iziTask.h>
#include <core/iziMemory.h>
#include <driver/iziAdc.h>
#include <driver/iziPin.h>
#include <driver/iziTc1.h>
#include <driver/iziTc2.h>
#include <utils/iziConsole.h>
#include <core/iziMutex.h>

void iziKernelIdleHook()
{
}

void iziKernelStartupHook()
{

}

void memory()
{
	iziConsolePrintf("%d\n", iziGetUsedMemory());
}

void blinkLed()
{
	iziPinSetType(eIziPinA5, eIziPinOut);

	while(1)
	{
		iziPinSet(eIziPinA5, IziTrue);
		iziTaskDelay(50);
		iziPinSet(eIziPinA5, IziFalse);
		iziTaskDelay(150);
		iziPinSet(eIziPinA5, IziTrue);
		iziTaskDelay(80);
		iziPinSet(eIziPinA5, IziFalse);
		iziTaskDelay(100);
		iziPinSet(eIziPinA5, IziTrue);
		iziTaskDelay(200);
		iziPinSet(eIziPinA5, IziFalse);
		iziTaskDelay(400);
		iziPinSet(eIziPinA5, IziTrue);
		break;
	}
}

int iziMain()
{
	iziConsoleInit(115200, 256);
	iziConsoleAttachCommand("mem", memory);
	iziConsoleAttachCommand("ls", iziConsolePrintCmdList);

	iziTaskCreate(blinkLed, eIziPrioLow, 96, 0);

	iziMutexGive(NULL);
	iziKernelStart();
	return 0;
}
