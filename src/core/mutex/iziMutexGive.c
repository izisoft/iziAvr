#include <device/iziDevicePriv.h>
#include <core/iziMutex.h>

#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>

IziBool_t iziMutexGive(TIziMutex* mutex)
{
	IziBool_t retVal = IziFalse;

	IZI_ATOMIC_BLOCK()
	{
		if((mutex->_value == 0) && (mutex->_owner == gIziCurrentTask))
		{
			mutex->_value = 1;
			gIziCurrentTask->_priority = mutex->_ownerPriority;
			// TODO moving current task to the end of list of original priority
			if((mutex->_subscribers._iter != NULL) &&
					(iziTaskWakeUpWithEvent((TIziTaskList *)(&(mutex->_subscribers)))
							>= mutex->_ownerPriority))
			{
				iziKernelYeld();
			}
			retVal = IziTrue;
		}
	}
	return retVal;
}
