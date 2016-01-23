#include <iziAvr.h>

#include <core/iziQueue.h>
#include <core/iziMemory.h>

IziBool_t iziQueueCreate(TIziQueue* queue, uint8_t itemCount, uint8_t itemSize)
{
	// Allocate memory for queue buffer
	queue->_buf = iziMalloc(itemCount*itemSize);
	if(queue->_buf != NULL)
	{
		queue->_iterWrite = queue->_buf;
		queue->_iterRead = queue->_buf;
		queue->_bufEnd = queue->_buf + itemCount*itemSize;

		queue->_itemSize = itemSize;
		queue->_itemCount = 0;
		queue->_maxItemCount = itemCount;

		queue->_waitToWriteList._iter = NULL;
		queue->_waitToReadList._iter = NULL;

		return IziTrue;
	}
	else
	{
		return IziFalse;
	}
}
