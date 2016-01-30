#include <avr/io.h>
#include <avr/interrupt.h>

#include <izi/avr/types.h>
#include <izi/avr/driver/adc.h>
#include <izi/avr/core/kernel.h>
#include <izi/avr/core/mutex.h>
#include <izi/avr/core/semaphore.h>

#define ENABLE_ADC_INT()			ADCSRA |= _BV(ADIE)
#define DISABLE_ADC_INT()			ADCSRA &= ~_BV(ADIE)

TIziMutex gIziAdcMutex = IZI_MUTEX_INITIALIZER;
TIziSemaphore gIziAdcSemaphore = IZI_SEMAPHORE_INITIALIZER(0);

void iziAdcConfigure(EIziAdcPrescaler prescaler, EIziAdcReference reference)
{
	//Choose voltage reference
	ADMUX &= 0xC0;
	ADMUX |= (reference << 6);

	//Choose ADC clock source and enable the converter
	ADCSRA &= 0x07;
	ADCSRA |= prescaler;
}

void iziAdcEnable()
{
	ADCSRA |= _BV(ADEN);
}

void iziAdcDisable()
{
	ADCSRA &= ~_BV(ADEN);
}

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
