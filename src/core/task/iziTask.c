#include <izi/avr/core/memory.h>

#include <core/kernel/iziKernelPriv.h>
#include <device/iziDevicePriv.h>

#include "iziTaskPriv.h"

//==============================================================================
// ACTIVE TASK LIST MAINIPULATION
//----------------------------------------------------------------------------
static void iziActiveTaskListAdd(TIziTask* task)
{
	TIziTaskList* list = (TIziTaskList*)&gIziActiveTaskList[task->_priority];
    TIziTask* iter = list->_iter;

    if(iter == NULL)
    {
    	list->_iter = task;
        task->_next = task;
        task->_prev = task;
    }
    else
    {
        task->_next = iter;
        task->_prev = iter->_prev;
        iter->_prev->_next = task;
        iter->_prev = task;
    }
    task->_owner = list;
#if IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_FTS
    ((TIziScoredTaskList*)list)->_taskCount++;
#endif
}
//------------------------------------------------------------------------------
void iziActiveTaskListRemove(TIziTask* task)
{
    if(task->_next != task)
    {
        task->_prev->_next = task->_next;
        task->_next->_prev = task->_prev;
        if(task->_owner->_iter == task)
        {
        	task->_owner->_iter = task->_next;
        }
    }
    else
    {
    	task->_owner->_iter = NULL;
    }
#if IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_FTS
    ((TIziScoredTaskList*)(task->_owner))->_taskCount--;
#endif
}
//==============================================================================
// INACTIVE TASK LIST MAINIPULATION
//----------------------------------------------------------------------------
static void iziTaskListAdd(TIziTaskList* list, TIziTask* task)
{
    TIziTask* head = list->_iter;
    if(head == NULL)
    {
        task->_next = NULL;
    }
    else
    {
        task->_next = head;
        head->_prev = task;
    }
    list->_iter = task;
    task->_prev = NULL;
    task->_owner = list;
}

static void iziTaskListRemove(TIziTask* task)
{
    if(task->_prev != NULL)
    {
        task->_prev->_next = task->_next;
    }
    if(task->_next != NULL)
    {
        task->_next->_prev = task->_prev;
    }
    if(task->_owner->_iter == task)
    {
    	task->_owner->_iter = task->_next;
    }
}

//==============================================================================
// EVENT LIST MAINIPULATION
//----------------------------------------------------------------------------
void iziTaskEventListAdd(TIziTaskList* list, TIziTask* task)
{
    TIziTask* iter = list->_iter;
    uint8_t priority = task->_priority;

    if((iter == NULL) || (iter->_priority < priority))
    {
        list->_iter = task;
        task->_enext = iter;
    }
    else
    {
        for(;;iter = iter->_enext)
        {
            if((iter->_enext == NULL) || (iter->_enext->_priority < priority))
            {
                task->_enext = iter->_enext;
                iter->_enext = task;
                break;
            }
        }
    }
    task->_eowner = list;
}
//------------------------------------------------------------------------------
static void iziTaskEventListRemove(TIziTask* task)
{
    TIziTask* iter = task->_eowner->_iter;
    if(iter == task)
    {
    	task->_eowner->_iter = task->_enext;
    }
    else
    {
        while(iter != NULL)
        {
            if(iter->_enext == task)
            {
                iter->_enext = task->_enext;
                break;
            }
            iter = iter->_enext;
        }
    }
    task->_eowner = NULL;
}
//==============================================================================
// LIST MANIPULATION INTERFACE
//----------------------------------------------------------------------------
void iziTaskActivate(TIziTask* task)
{
	iziTaskListRemove(task);
	iziActiveTaskListAdd(task);
}

void iziTaskWait()
{
	if(gIziCurrentTask != gIziKernelTask)
	{
		iziActiveTaskListRemove((TIziTask*)gIziCurrentTask);
		iziTaskListAdd((TIziTaskList *)(&gIziWaitingTaskList), (TIziTask*)gIziCurrentTask);
	}
}

void iziTaskSuspend(IziDelay_t waitTicks)
{
	if(gIziCurrentTask != gIziKernelTask)
	{
		iziActiveTaskListRemove((TIziTask*)gIziCurrentTask);
		if (waitTicks != IZI_MAX_DELAY)
		{
			gIziCurrentTask->_wakeUpTick = gIziTime + waitTicks;
			iziTaskListAdd((TIziTaskList *)(&gIziWaitingTaskList), (TIziTask*)gIziCurrentTask);
		}
		else
		{
			iziTaskListAdd((TIziTaskList *)(&gIziSuspendedTaskList), (TIziTask*)gIziCurrentTask);
		}
	}
}

//------------------------------------------------------------------------------
EIziTaskPriority iziTaskWakeUpWithEvent(TIziTaskList* list)
{
	EIziTaskPriority wakeUpTaskPriority = list->_iter->_priority;

	iziTaskActivate(list->_iter);
	iziTaskEventListRemove(list->_iter);

	return wakeUpTaskPriority;
}
//==============================================================================
// TASK MAINIPULATION
//----------------------------------------------------------------------------
TIziTask* iziTaskCreate(void (*taskFunction)(void), EIziTaskPriority priority,
	uint16_t stackSize, void* param)
{
	TIziTask* task = NULL;

	task = iziMalloc(sizeof(TIziTask)+stackSize);
	task->_stackPointer = (uint8_t*)task + (sizeof(TIziTask)+ stackSize - 1);
	task->_priority = (uint8_t)priority;
	task->_wakeUpTick = 0;
	task->_eowner = NULL;
#if IZI_KERNEL_TYPE > IZI_KERNEL_TINY
	task->_params = param;
#endif

	iziDeviceInitTaskStack(task, taskFunction);

	if(IZI_GET_STATE(eIziKernelStarted))
	{
		IZI_ATOMIC_INSTRUCTION(iziActiveTaskListAdd(task));
	}
	else
	{
		iziTaskListAdd((TIziTaskList*)(&gIziSuspendedTaskList),task);
	}
	return task;
}

void iziTaskFinalize()
{
	iziActiveTaskListRemove((TIziTask*)gIziCurrentTask);
	iziTaskListAdd((TIziTaskList *)(&gIziRemovedTaskList), (TIziTask*)gIziCurrentTask);
	for(;;);
}
