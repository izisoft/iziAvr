#include <string.h>

#include <core/kernel/iziKernelPriv.h>
#include <core/task/iziTaskPriv.h>
#include <device/iziDevicePriv.h>

#include "iziQueuePriv.h"

uint8_t iziQueueGetFromIsr(TIziQueue* queue, void* item, uint8_t* needYeld)
{
	*needYeld = IziFalse;
	if(queue->_itemCount > 0)
	{
		iziQueueGetItem(queue, item);

		if((queue->_waitToWriteList._iter != NULL) &&
				(iziTaskWakeUpWithEvent((TIziTaskList *)(&(queue->_waitToWriteList))) > gIziCurrentTask->_priority))
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
