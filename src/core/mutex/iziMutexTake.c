#include <izi/avr/core/mutex.h>

#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>
#include <device/iziDevicePriv.h>

IziBool_t iziMutexTake(TIziMutex* mutex, IziDelay_t waitTicks)
{
	IZI_ATOMIC_BLOCK()
	{
		if((mutex->_value == 0) && (waitTicks > 0))
		{
			iziTaskSuspend(waitTicks);
			iziTaskEventListAdd((TIziTaskList*)(&(mutex->_subscribers)), (TIziTask*)gIziCurrentTask);
			if(gIziCurrentTask->_priority > mutex->_owner->_priority)
			{
				mutex->_owner->_priority = gIziCurrentTask->_priority;
			}
			iziKernelYeld();
		}

		if(mutex->_value == 1)
		{
			mutex->_value = 0;
			mutex->_ownerPriority = gIziCurrentTask->_priority;
			mutex->_owner = (TIziTask*)gIziCurrentTask;
			return IziTrue;
		}
	}
	return IziFalse;
}
