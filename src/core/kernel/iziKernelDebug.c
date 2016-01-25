#include <core/task/iziTaskPriv.h>
#include <device/iziDevicePriv.h>

#ifdef IZI_KERNEL_DEBUG

volatile uint16_t gIziKernelDebugSecond = 0;
volatile uint32_t gIziKernelDebugYeldCount = 0;

void iziKernelDebugForeachTask(void (*callback)(TIziTask*))
{
	TIziTask* end;
	TIziTask* task;
	for(uint8_t i = 0; i < IZI_PRIORITY_COUNT; ++i)
	{
		task = end = gIziActiveTaskList[i]._iter;
		if(task != NULL)
		{
			do
			{
				callback(task);
				task = task->_next;
			}
			while(task != end);
		}
	}
	task = gIziWaitingTaskList._iter;
	while(task != NULL)
	{
		callback(task);
		task = task->_next;
	}
	task = gIziSuspendedTaskList._iter;
	while(task != NULL)
	{
		callback(task);
		task = task->_next;
	}
}

void iziKernelDebugStackUtilization(TIziTask* task)
{
	if(task->_stackPointer > (uint8_t*)task + sizeof(TIziTask))
	{
		IziSize_t remaining = task->_stackPointer - (uint8_t*)task - sizeof(TIziTask);
		task->_stackUtilization = 0x00FF * (task->_stackSize - remaining) / task->_stackSize;
	}
	else
	{
		task->_stackUtilization = 0xFF;
	}
}

void iziKernelDebugResetYeldCount(TIziTask* task)
{
	task->_cpuUtilization = (uint32_t)(0x00FF * task->_yeldCount) / gIziKernelDebugYeldCount;
	task->_yeldCount = 0;
}

void iziKernelDebugYeldHook()
{
	iziKernelDebugForeachTask(iziKernelDebugStackUtilization);

	if(gIziTime.rts.sec == gIziKernelDebugSecond)
	{
		++gIziKernelDebugYeldCount;
		++gIziCurrentTask->_yeldCount;
	}
	else
	{
		iziKernelDebugForeachTask(iziKernelDebugResetYeldCount);
		gIziKernelDebugYeldCount = 0;
		gIziKernelDebugSecond = gIziTime.rts.sec;
	}
}

#endif
