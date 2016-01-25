#ifndef IZI_QUEUEPRIV_H_
#define IZI_QUEUEPRIV_H_

#include <string.h>

#include <izi/avr/core/queue.h>

static inline void iziQueueGetItem(TIziQueue* queue, void* item)
{
	memcpy(item,(const void *)(queue->_iterRead),queue->_itemSize);
	queue->_iterRead += queue->_itemSize;
	if(queue->_iterRead == queue->_bufEnd)
	{
		queue->_iterRead = queue->_buf;
	}
	queue->_itemCount--;
}

static inline void iziQueuePutItem(TIziQueue* queue, const void* item)
{
	memcpy((void *)(queue->_iterWrite),item,queue->_itemSize);
	queue->_iterWrite += queue->_itemSize;
	if(queue->_iterWrite == queue->_bufEnd)
	{
		queue->_iterWrite = queue->_buf;
	}
	queue->_itemCount++;
}

#endif /* IZI_QUEUEPRIV_H_ */
