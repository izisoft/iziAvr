#include <avr/io.h>
#include <avr/interrupt.h>

#include <izi/avr/driver/tc0.h>

#include <device/iziDevicePriv.h>

void iziTc0Init(ETc0ClockSource cs, ETc0WaveGenerationMode wgm, ETc0OutCmpMode ocm)
{
	TCCR0 = (cs & 0x07) | ((wgm & 0x01) << WGM00) | ((wgm & 0x02) << (WGM01-1)) | ((ocm & 0x03)  << 4);
}

void iziTc0SetOutputCmpValue(IziTc0Value_t value)
{
	OCR0 = value;
}

void iziTc0SetCounterValue(IziTc0Value_t value)
{
	TCNT0 = value;
}

#if IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC0

IziIsrHandler_t gTc0IsrHandlers[TC0_ISR_COUNT] = {NULL};

void iziTc0EnableInterrupt(ETc0IntSource intSource)
{
	TIMSK |= (1 << intSource);
}

void iziTc0DisableInterrupt(ETc0IntSource intSource)
{
	TIMSK &= ~(1 << intSource);
}

void iziTc0DetachHandler(ETc0IntSource intSource)
{
	gTc0IsrHandlers[intSource] = NULL;
}

void iziTc0AttachHandler(ETc0IntSource intSource, IziIsrHandler_t handler)
{
	gTc0IsrHandlers[intSource] = handler;
}

ISR(TIMER0_OVF_vect)
{
	if(gTc0IsrHandlers[eIziTc0Int_ovf])
	{
		gTc0IsrHandlers[eIziTc0Int_ovf]();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(gTc0IsrHandlers[eIziTc0Int_ocmp])
	{
		gTc0IsrHandlers[eIziTc0Int_ocmp]();
	}
}
#endif
