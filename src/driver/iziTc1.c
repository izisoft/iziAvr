#include <avr/io.h>
#include <avr/interrupt.h>

#include <izi/avr/driver/tc1.h>

#include <device/iziDevicePriv.h>

void iziTc1Init(ETc1ClockSource cs, ETc1WaveGenerationMode wgm,
		ETc1OutCmpMode ocmA, ETc1OutCmpMode ocmB)
{
	TCCR1A = ((ocmA & 0x03) << 6) | ((ocmB & 0x03) << 4) | (wgm & 0x03);
	TCCR1B = (cs & 0x07) | ((wgm << 1) & (0x18));
}

void iziTc1SetOutputCmpValueA(IziTc1Value_t value)
{
	OCR1A = value;
}

void iziTc1SetOutputCmpValueB(IziTc1Value_t value)
{
	OCR1B = value;
}

void iziTc1SetCounterValue(IziTc1Value_t value)
{
	TCNT1 = value;
}

#if (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC1A) && (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC1B)

IziIsrHandler_t gTc1IsrHandlers[TC1_ISR_COUNT] = {NULL};

void iziTc1EnableInterrupt(ETc1IntSource intSource)
{
	TIMSK |= (0x04 << intSource);
}

void iziTc1DisableInterrupt(ETc1IntSource intSource)
{
	TIMSK &= ~(0x04 << intSource);
}

void iziTc1AttachHandler(ETc1IntSource intSource, IziIsrHandler_t handler)
{
	gTc1IsrHandlers[intSource] = handler;
}

void iziTc1DetachHandler(ETc1IntSource intSource)
{
	gTc1IsrHandlers[intSource] = NULL;
}

//ISR(TIMER1_OVF_vect)
//{
//	if(gTc1IsrHandlers[eIziTc1Int_ovf])
//	{
//		gTc1IsrHandlers[eIziTc1Int_ovf]();
//	}
//}
//
//ISR(TIMER1_CAPT_vect)
//{
//	if(gTc1IsrHandlers[eIziTc1Int_icpt])
//	{
//		gTc1IsrHandlers[eIziTc1Int_icpt]();
//	}
//}
//
//ISR(TIMER1_COMPA_vect)
//{
//	if(gTc1IsrHandlers[eIziTc1Int_ocmpA])
//	{
//		gTc1IsrHandlers[eIziTc1Int_ocmpA]();
//	}
//}
//
//ISR(TIMER1_COMPB_vect)
//{
//	if(gTc1IsrHandlers[eIziTc1Int_ocmpB])
//	{
//		gTc1IsrHandlers[eIziTc1Int_ocmpB]();
//	}
//}
#endif
