#include <izi/avr/core/semaphore.h>

#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>
#include <device/iziDevicePriv.h>

IziBool_t iziSemaphoreTake(TIziSemaphore* semaphore,IziDelay_t waitTicks)
{
	IZI_ATOMIC_BLOCK()
	{
		if((semaphore->_value == 0) && (waitTicks > 0))
		{
			iziTaskSuspend(waitTicks);
			iziTaskEventListAdd((TIziTaskList *)(&(semaphore->_subscribers)),(TIziTask*)gIziCurrentTask);
			iziKernelYeld();
		}

		if(semaphore->_value > 0)
		{
			semaphore->_value--;
			return IziTrue;
		}
	}
	return IziFalse;
}
