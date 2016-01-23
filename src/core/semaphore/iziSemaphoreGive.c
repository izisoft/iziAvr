#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>
#include <device/iziDevicePriv.h>

#include <core/iziSemaphore.h>

IziBool_t iziSemaphoreGive(TIziSemaphore* semaphore)
{
	IziBool_t retVal = IziFalse;
	IZI_ATOMIC_BLOCK()
	{
		if(semaphore->_value == 0)
		{
			semaphore->_value = 1;
			if((semaphore->_subscribers._iter != NULL) &&
				(iziTaskWakeUpWithEvent((TIziTaskList *)(&(semaphore->_subscribers)))
						> gIziCurrentTask->_priority))
			{
				iziKernelYeld();
			}
			retVal = IziTrue;
		}
	}
	return retVal;
}
