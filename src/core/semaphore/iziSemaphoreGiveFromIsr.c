#include <iziAvr.h>

#include <core/iziSemaphore.h>
#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>

IziBool_t iziSemaphoreGiveFromIsr(TIziSemaphore* semaphore, uint8_t* needYeld)
{
	*needYeld = IziFalse;
	if(semaphore->_value == 0)
	{
		semaphore->_value = 1;
		if((semaphore->_subscribers._iter != NULL) &&
			(iziTaskWakeUpWithEvent((TIziTaskList *)(&(semaphore->_subscribers)))
					> gIziCurrentTask->_priority))
		{
			*needYeld = IziTrue;
		}
		return IziTrue;
	}
	else
	{
		return IziFalse;
	}
}
