#include <core/task/iziTaskPriv.h>
#include <core/iziMemory.h>
#include <device/iziDevicePriv.h>

#include "iziKernelPriv.h"

#if IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_FTS
#define IZI_ACTIVE_TASK_LIST_INITIALIZER {NULL, 0, 0}
#else
#define IZI_ACTIVE_TASK_LIST_INITIALIZER {NULL}
#endif

volatile TIziTask* gIziCurrentTask = NULL;
volatile TIziTask* gIziKernelTask = NULL;
volatile TIziActiveTaskList gIziActiveTaskList[IZI_PRIORITY_COUNT] = {IZI_ACTIVE_TASK_LIST_INITIALIZER};
volatile TIziTaskList gIziWaitingTaskList = {NULL};
volatile TIziTaskList gIziSuspendedTaskList = {NULL};
volatile TIziTaskList gIziRemovedTaskList = {NULL};
volatile IziTime_t gIziTime;
volatile uint16_t gIziState = 0;

//==============================================================================
// DEFINITIONS
#define IZI_KERNEL_TASK_STACK (128)

//==============================================================================
// SCHEDULERS
//----------------------------------------------------------------------------
#if IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_HPF
// HPF scheduler (High Priority First)
static void iziKernelSwitchContext()
{
	for(uint8_t i = IZI_PRIORITY_COUNT - 1; i >= 0; i--)
	{
		if(gIziActiveTaskList[i]._iter != NULL)
		{
			gIziCurrentTask = gIziActiveTaskList[i]._iter;
			gIziActiveTaskList[i]._iter = gIziActiveTaskList[i]._iter->_next;
			break;
		}
	}
}
#endif
//------------------------------------------------------------------------------
#if IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_FTS
// FTS scheduler (Fair Time Share)
static void iziKernelSwitchContext()
{
	uint8_t j = gIziCurrentTask->_priority;

	gIziActiveTaskList[j]._score = (gIziActiveTaskList[j]._taskCount)+(j*j);

	j = 0;
	for(uint8_t i = IZI_PRIORITY_COUNT - 1; i >= 0; i--)
	{
		if(gIziActiveTaskList[i]._iter != NULL)
		{
			gIziActiveTaskList[i]._score = (gIziActiveTaskList[i]._score + 1) & 0x7F;
			if(gIziActiveTaskList[i]._score > j)
			{
				gIziCurrentTask = gIziActiveTaskList[i]._iter;
				j = gIziActiveTaskList[i]._score;
			}
		}
	}
	gIziCurrentTask->_owner->_iter = gIziCurrentTask->_owner->_iter->_next;
}
#endif
//==============================================================================
// YELD UTILITIES
//----------------------------------------------------------------------------
static void iziKernelCheckWaitingTasks()
{
	TIziTask* iter = gIziWaitingTaskList._iter;
	while(iter != NULL)
	{
		if(gIziTime >= iter->_wakeUpTick)
		{
			iziTaskActivate(iter);
			iter = gIziWaitingTaskList._iter;
		}
		else
		{
			iter = iter->_next;
		}
	}
}

//------------------------------------------------------------------------------
static void iziKernelWakeAllSuspendedTasks()
{
	IZI_ATOMIC_BLOCK()
	{
		while(gIziSuspendedTaskList._iter != NULL)
		{
			iziTaskActivate(gIziSuspendedTaskList._iter);
		}
#if IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_FTS
		for(uint8_t i = 0; i < IZI_PRIORITY_COUNT; i++)
		{
			gIziActiveTaskList[i]._score = (gIziActiveTaskList[i]._taskCount)+(i*i);
		}
#endif
	}
}
//------------------------------------------------------------------------------
#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_TINY
static void iziKernelCheckStack()
{
	if(gIziCurrentTask->_stackPointer < (uint8_t*)(&(gIziCurrentTask->_params)+1))
	{
		IZI_FATAL(eIziStackOverflow);
	}
}
#endif
//==============================================================================
// YELD METHODS
//----------------------------------------------------------------------------
void iziKernelYeldFromTick( void ) __attribute__ ( ( naked ) );
void iziKernelYeldFromTick( void )
{
	IZI_SAVE_CONTEXT();
	IZI_SET_KERNEL_STACK();
#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_TINY
	iziKernelCheckStack();
#endif
	++gIziTime;
	iziKernelCheckWaitingTasks();
	iziKernelSwitchContext();
	IZI_RESTORE_CONTEXT();
	IZI_RETURN();
}

//------------------------------------------------------------------------------
void iziKernelYeld( void ) __attribute__ ( ( naked ) );
void iziKernelYeld( void )
{
	IZI_SAVE_CONTEXT();
	IZI_SET_KERNEL_STACK();
#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_TINY
	iziKernelCheckStack();
#endif
	iziKernelSwitchContext();
	IZI_RESTORE_CONTEXT();
	IZI_RETURN();
}
//------------------------------------------------------------------------------
IZI_SYSTEM_INTERRUPT()
{
	iziKernelYeldFromTick();
	IZI_RETURN_INTERRUPT();
}
//==============================================================================
// SYSTEM TASK
//----------------------------------------------------------------------------
static void iziKernelCleanup()
{
	TIziTask* task;
	IZI_ATOMIC_BLOCK()
	{
		while(gIziRemovedTaskList._iter != NULL)
		{
			task = gIziRemovedTaskList._iter;
			gIziRemovedTaskList._iter = task->_next;
			iziFree((void*)task);
		}
	}
}

void iziKernelTask()
{
	// Startup
	iziKernelStartupHook();
	iziKernelWakeAllSuspendedTasks();
	iziKernelSetState(eIziKernelStarted);
	iziKernelYeld();

	// Idle loop
	while(1)
	{
		iziKernelIdleHook();
		iziKernelCleanup();
		iziKernelYeld();
	}
}

//==============================================================================
// KERNEL START METHOD
//----------------------------------------------------------------------------
void iziKernelStart()
{
	// Create kernel idle and startup task
	gIziKernelTask = iziTaskCreate(iziKernelTask,
					eIziPrioLow,
					IZI_KERNEL_TASK_STACK,
					NULL);
	iziTaskActivate((TIziTask*)gIziKernelTask);

	// Initialize system time
	iziDeviceInitSystemClock();

	gIziState |= _BV(eIziKernelInit);

	// Start scheduler
	IZI_DISABLE_INTERRUPTS();
	IZI_ENABLE_SYSTEM_INTERRUPT();
	gIziCurrentTask = gIziKernelTask;
	IZI_RESTORE_CONTEXT();
	IZI_RETURN();
	// loop forever, should never come back here
	for(;;);
}
