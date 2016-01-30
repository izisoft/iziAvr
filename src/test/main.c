#include <avr/io.h>
#include <avr/interrupt.h>

#include <izi/avr/core.h>
#include <izi/avr/core/memory.h>
#include <izi/avr/driver/adc.h>
#include <izi/avr/driver/pin.h>
#include <izi/avr/utils/console.h>

void iziKernelIdleHook()
{
}

void iziKernelStartupHook()
{

}

static uint16_t gDelay = 0;

void blink()
{
	iziPinSetType(eIziPinA5, eIziPinOut);

    for(;;)
    {
        iziPinToggle(eIziPinA5);
        iziTaskDelayMs(gDelay);
    }
}

void gauge()
{
	uint16_t value = 0;

	iziPinSetType(eIziPinA0, eIziPinIn);

	iziAdcConfigure(eIziAdcClkDiv2, eIziAdcRefAvcc);
	iziAdcEnable();

	for(;;)
	{
		value = iziAdcReadChannel(eIziAdcCh0);
		IZI_ATOMIC_INSTRUCTION(gDelay = value);
		iziTaskDelayMs(100);
	}
}

int iziMain()
{
	iziTaskCreate(blink, eIziTaskPrioLow, 96, 0);
	iziTaskCreate(gauge, eIziTaskPrioLow, 96, 0);

	iziKernelStart();
	return 0;
}
