#include <core/kernel/iziKernelPriv.h>

#include "iziTaskPriv.h"

void iziTaskDelayMs(IziDelay_t waitMs)
{
	IZI_ATOMIC_BLOCK()
	{
		gIziCurrentTask->_wakeUpTick = gIziTime + (((uint32_t)(waitMs) * IZI_SYSTEM_TICK_RATE + 500) / 1000);
		iziTaskWait();
	}
	iziKernelYeld();
}
