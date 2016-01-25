#include <string.h>

#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>
#include <device/iziDevicePriv.h>

#include "iziQueuePriv.h"

uint8_t iziQueueGetFromTask(TIziQueue* queue, void* item, IziDelay_t waitTicks)
{
	IZI_ATOMIC_BLOCK()
	{
		if((queue->_itemCount == 0) && (waitTicks > 0))
		{
			iziTaskSuspend(waitTicks);
			iziTaskEventListAdd((TIziTaskList *)(&(queue->_waitToReadList)),(TIziTask*)gIziCurrentTask);
			iziKernelYeld();
		}

		if(queue->_itemCount > 0)
		{
			iziQueueGetItem(queue, item);
			if((queue->_waitToWriteList._iter != NULL) &&
				(iziTaskWakeUpWithEvent((TIziTaskList *)(&(queue->_waitToWriteList)))
						> gIziCurrentTask->_priority))
			{
				iziKernelYeld();
			}
			return IziTrue;
		}
	}
	return IziFalse;
}
