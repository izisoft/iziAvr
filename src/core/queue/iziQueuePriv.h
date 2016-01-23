#ifndef IZIQUEUEPRIV_H_
#define IZIQUEUEPRIV_H_

#include <string.h>

#include <core/iziQueue.h>

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

#endif /* IZIQUEUEPRIV_H_ */
