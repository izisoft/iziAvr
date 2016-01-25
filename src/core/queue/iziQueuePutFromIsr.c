#include <string.h>

#include <device/iziDevicePriv.h>
#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>

#include "iziQueuePriv.h"

uint8_t iziQueuePutFromIsr(TIziQueue *queue,const void* item,uint8_t *needYeld)
{
	*needYeld = IziFalse;
	if(queue->_itemCount < queue->_maxItemCount)
	{
		iziQueuePutItem(queue, item);
		if((queue->_waitToReadList._iter != NULL) &&
				(iziTaskWakeUpWithEvent((TIziTaskList *)(&(queue->_waitToReadList))) > gIziCurrentTask->_priority))
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
