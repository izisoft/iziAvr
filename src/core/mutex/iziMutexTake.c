#include <device/iziDevicePriv.h>
#include <core/iziMutex.h>
#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>

IziBool_t iziMutexTake(TIziMutex* mutex, IziDelay_t waitTicks)
{
	IZI_LOOP_CRITICAL()
	{
		if(mutex->_value == 1)
		{
			mutex->_value = 0;
			mutex->_ownerPriority = gIziCurrentTask->_priority;
			mutex->_owner = (TIziTask*)gIziCurrentTask;
			IZI_RETURN_CRITICAL(IziTrue);
		}
		else if (waitTicks == 0)
		{
			IZI_RETURN_CRITICAL(IziFalse);
		}
		else
		{
			iziTaskSuspend(waitTicks);
			iziTaskEventListAdd((TIziTaskList*)(&(mutex->_subscribers)), (TIziTask*)gIziCurrentTask);
			if(gIziCurrentTask->_priority > mutex->_owner->_priority)
			{
				mutex->_owner->_priority = gIziCurrentTask->_priority;
			}
			iziKernelYeld();
		}

		if(mutex->_value == 0)
		{
			IZI_RETURN_CRITICAL(IziFalse);
		}
	}
	return IziFalse;
}
