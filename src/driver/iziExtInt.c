#include <avr/io.h>
#include <avr/interrupt.h>

#include <iziAvr.h>
#include <driver/iziExtInt.h>

void (*iziExtIntHandlerTable[IZI_EXT_INT_COUNT])(void);

//=====================================================================
void iziExtInt0SetSense(TIziExtInt0Sense sense)
{
	MCUCR &= ~(0x03);
	MCUCR |= sense;
}
//--
void iziExtInt1SetSense(TIziExtInt1Sense sense)
{
	MCUCR &= ~(0x0C);
	MCUCR |= ((sense << 2) & 0x0C);
}
//--
void iziExtInt2SetSense(TIziExtInt2Sense sense)
{
	MCUCR &= ~(0x40);
	MCUCR |= ((sense << 6) & 0x40);
}

//=====================================================================
void iziExtIntEnableInterrupt(TIziExtIntSource intSource)
{
	GICR |= (0x20 << intSource);
}
//--
void iziExtIntDisableInterrupt(TIziExtIntSource intSource)
{
	GICR &= ~(0x20 << intSource);
}

//=====================================================================
void iziExtIntDetachHandler(TIziExtIntSource intSource)
{
	iziExtIntHandlerTable[intSource] = NULL;
}
//--
void iziExtIntAttachHandler(TIziExtIntSource intSource, void (*handler)(void))
{
	iziExtIntHandlerTable[intSource] = handler;
}

//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// Interrupt routines
ISR(INT0_vect)
{
	if(iziExtIntHandlerTable[eIziExtInt0]) {
		iziExtIntHandlerTable[eIziExtInt0]();
	}
}
ISR(INT1_vect)
{
	if(iziExtIntHandlerTable[eIziExtInt1]) {
		iziExtIntHandlerTable[eIziExtInt1]();
	}
}
ISR(INT2_vect)
{
	if(iziExtIntHandlerTable[eIziExtInt2]) {
		iziExtIntHandlerTable[eIziExtInt2]();
	}
}
