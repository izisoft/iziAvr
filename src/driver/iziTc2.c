#include <avr/io.h>
#include <avr/interrupt.h>

#include <device/iziDevicePriv.h>
#include <driver/iziTc2.h>
#include <utils/iziPreprocessor.h>

#define IZI_TC2_ASYNC_MODE() (ASSR & _BV(AS2))

void iziTc2Init(ETc2ClockSource cs, ETc2WaveGenerationMode wgm, ETc2OutCmpMode ocm, IziBool_t async)
{
	TCCR2 = (cs & 0x07) | ((wgm & 0x01) << WGM20) | ((wgm & 0x02) << (WGM21-1)) | ((ocm & 0x03)  << 4);
	if(async == IziTrue)
	{
		ASSR |= _BV(AS2);
	}
	else
	{
		ASSR &= ~_BV(AS2);
	}
}

void iziTc2SetOutputCmpValue(IziTc2Value_t value)
{
	OCR2 = value;
}

void iziTc2SetCounterValue(IziTc2Value_t value)
{
	TCNT2 = value;
}

#if (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC2) && (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC2_ASYNC)

IziIsrHandler_t gTc2IsrHandlers[TC2_ISR_COUNT] = {NULL};

void iziTc2EnableInterrupt(ETc2IntSource intSource)
{
	TIMSK |= (0x40 << intSource);
}

void iziTc2DisableInterrupt(ETc2IntSource intSource)
{
	TIMSK &= ~(0x40 << intSource);
}

void iziTc2AttachHandler(ETc2IntSource intSource, IziIsrHandler_t handler)
{
	gTc2IsrHandlers[intSource] = handler;
}

void iziTc2DetachHandler(ETc2IntSource intSource)
{
	gTc2IsrHandlers[intSource] = NULL;
}

ISR(TIMER2_OVF_vect)
{
	if(gTc2IsrHandlers[eIziTc2Int_ovf])
	{
		gTc2IsrHandlers[eIziTc2Int_ovf]();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(gTc2IsrHandlers[eIziTc2Int_ocmp])
	{
		gTc2IsrHandlers[eIziTc2Int_ocmp]();
	}
}

#endif
