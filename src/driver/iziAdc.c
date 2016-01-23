#include <avr/io.h>
#include <avr/interrupt.h>

#include <iziAvr.h>
#include <driver/iziAdc.h>
#include <core/iziKernel.h>
#include <core/iziMutex.h>
#include <core/iziSemaphore.h>

#define ENABLE_ADC_INT()			ADCSRA |= _BV(ADIE)
#define DISABLE_ADC_INT()			ADCSRA &= ~_BV(ADIE)

TIziMutex gIziAdcMutex;
TIziSemaphore gIziAdcSemaphore;

//=====================================================================
void iziAdcInit()
{
	iziMutexCreate(&gIziAdcMutex);
	iziSemaphoreCreate(&gIziAdcSemaphore,IziFalse);
}

void iziAdcConfigure(EIziAdcPrescaler prescaler, EIziAdcReference reference)
{
	//Choose voltage reference
	ADMUX &= 0xC0;
	ADMUX |= (reference << 6);

	//Choose ADC clock source and enable the converter
	ADCSRA &= 0x07;
	ADCSRA |= prescaler;
}

//=====================================================================
void iziAdcEnable()
{
	ADCSRA |= _BV(ADEN);
}
//--
void iziAdcDisable()
{
	ADCSRA &= ~_BV(ADEN);
}

//=====================================================================
uint16_t iziAdcReadChannel(EIziAdcChannel channel)
{
	uint16_t retVal;
	if(iziMutexTake(&gIziAdcMutex,IZI_MAX_DELAY) == IziTrue)
	{
		//Choose a channel to be measured
		ADMUX &= 0xE0;
		ADMUX |= channel;

		//Enable ADC interrupt
		ENABLE_ADC_INT();
		//Start the measurement
		ADCSRA |= _BV(ADSC);

		//Wait for the conversion result
		iziSemaphoreTake(&gIziAdcSemaphore,IZI_MAX_DELAY);
		retVal = ADCW;
		iziMutexGive(&gIziAdcMutex);
		return retVal;
	}
	else
	{
		return 0xFFFF;
	}
}

//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
ISR(ADC_vect)
{
	IziBool_t needYeld;

	iziSemaphoreGiveFromIsr(&gIziAdcSemaphore,&needYeld);
	DISABLE_ADC_INT();
	if(needYeld == IziTrue)
	{
		iziKernelYeld();
	}
}
