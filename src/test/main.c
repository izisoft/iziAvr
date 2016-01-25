#include <avr/io.h>
#include <avr/interrupt.h>

#include <izi/avr/types.h>
#include <izi/avr/core/kernel.h>
#include <izi/avr/core/task.h>
#include <izi/avr/core/memory.h>
#include <izi/avr/driver/adc.h>
#include <izi/avr/driver/pin.h>
#include <izi/avr/driver/tc1.h>
#include <izi/avr/driver/tc2.h>
#include <izi/avr/utils/console.h>
#include <izi/avr/core/mutex.h>

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
