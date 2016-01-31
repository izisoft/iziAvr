#ifndef IZI_QUEUE_H_
#define IZI_QUEUE_H_

#include <izi/avr/core/task.h>
	
#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup queue
 * @{
 *
 * \file queue.h
 * Implementation of iziAvr library data oriented IPC system.
 * Queues allows to pass data streams between independent
 * processing units (tasks and ISRs).
 */

/*! \struct SIziQueue
 * Structure holding queue data.
 */
struct SIziQueue
{
	uint8_t* _buf;
	uint8_t* _bufEnd;
	uint8_t* _iterWrite;
	uint8_t* _iterRead;

	uint8_t _itemSize;
	uint8_t _itemCount;
	uint8_t _maxItemCount;

	TIziTaskList _waitToWriteList;
	TIziTaskList _waitToReadList;
};

/*! \typedef TIziQueue
 * Type defined over queue structure for convenience.
 */
typedef struct SIziQueue TIziQueue;

/*! Creates new queue instance. Function handles allocating needed space
 *	for queue buffer. 
 *	\param queue
 *	Pointer to queue structure. If it points to previously created queue,
 *	function will take care about releasing memory before allocating new block.
 *	\param itemCount
 *	Maximum number of items in queue.
 *	\param itemSize
 *	Size of single queue item in bytes.
 *	\return 
 *	If queue is successfully created then function returns IziTrue. If queue
 *	is not created IziFalse is returned.
 */
IziBool_t iziQueueCreate(TIziQueue *queue,uint8_t itemCount,uint8_t itemSize);

/*! Puts single item to queue from task.
 *	\param queue
 *	Pointer to queue where new item will be send.
 *	\param item
 *	Pointer to new queue item.
 *	\param waitTicks
 *	If queue is full, this param determines maximal number of system ticks 
 *	when task will wait for handling demand. Setting param to IZI_MAX_DELAY will
 *	suspend the task until queue is ready to receive new item.
 *	\return
 *	If item is handled by queue function will return IziTrue. If request is not
 *	fulfilled during set waiting time it will return IziFalse.
 */
IziBool_t iziQueuePutFromTask(TIziQueue *queue,const void* item,IziDelay_t waitTicks);

/*! Puts single item to queue from interrupt service routine. In contrast to
 *	putting item from task in ISR we can't wait if queue is full.
 *	\param queue
 *	Pointer to queue where new item will be send.
 *	\param item
 *	Pointer to new queue item.
 *	\param needYeld
 *	If there is a task waiting for queue item, and it has higher priority than
 *	currently running task this parameter will be set to IziTrue. Otherwise
 *	IziFalse value will be written to needYeld param.
 *	\return
 *	Function will return IziFalse when queue was full, and putting new item failed,
 *	IziTrue otherwise.
 */
IziBool_t iziQueuePutFromIsr(TIziQueue *queue,const void* item,uint8_t *needYeld);

/*! Gets single item from queue within task.
 *	\param queue
 *	Pointer to queue.
 *	\param item
 *	Pointer to item which will be filled with value from queue if operation succeeds.
 *	\param waitTicks
 *	If queue is empty task will wait not longer than number of ticks given in this
 *	parameter. When it is set to IZI_MAX_DELAY than task will be suspended until
 *	new item appears in queue.
 *	\return
 *	If item was taken from queue within prescribed time it will return IziTrue,
*	otherwise IziFalse will be given. 
 */
IziBool_t iziQueueGetFromTask(TIziQueue *queue,void* item,IziDelay_t waitTicks);

/*! Gets single item from queue within interrupt service routine.
 *	\param queue
 *	Pointer to queue.
 *	\param item
 *	Pointer to item which will be filled with value from queue if operation succeeds.
 *	\param needYeld
 *	If there is a task waiting to put item to queue, and it has higher priority than
 *	currently running task this parameter will be set to IziTrue. Otherwise
 *	IziFalse value will be written to needYeld param.
 *	\return
 *	IziFalse will be given as return value only when queue is empty, otherwise
 *	function will return IziTrue value.
 */
IziBool_t iziQueueGetFromIsr(TIziQueue *queue,void* item,uint8_t *needYeld);

//! @}

#ifdef __cplusplus
}
#endif

#endif /* IZI_QUEUE_H_ */
