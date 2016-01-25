#include <izi/avr/core/semaphore.h>
#include <izi/avr/types.h>

IziBool_t iziSemaphoreTaskeFromIsr(TIziSemaphore* semaphore)
{
	if(semaphore->_value != 0)
	{
		semaphore->_value--;
		return IziTrue;
	}
	else
	{
		return IziFalse;
	}
}
