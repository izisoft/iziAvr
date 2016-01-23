#include <string.h>

#include <device/iziDevicePriv.h>
#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>
#include <core/queue/iziQueuePriv.h>

uint8_t iziQueuePutFromTask(TIziQueue* queue,const void* item, IziDelay_t waitTicks)
{
	IZI_ATOMIC_BLOCK()
	{
		if((queue->_itemCount >= queue->_maxItemCount) && (waitTicks > 0))
		{
			iziTaskSuspend(waitTicks);
			iziTaskEventListAdd((TIziTaskList *)(&(queue->_waitToWriteList)), (TIziTask*)gIziCurrentTask);
			iziKernelYeld();
		}

		if(queue->_itemCount < queue->_maxItemCount)
		{
			iziQueuePutItem(queue, item);
			if((queue->_waitToReadList._iter != NULL) &&
				(iziTaskWakeUpWithEvent((TIziTaskList *)(&(queue->_waitToReadList)))
						> gIziCurrentTask->_priority))
			{
				iziKernelYeld();
			}
			return IziTrue;
		}
	}
	return IziFalse;
}
