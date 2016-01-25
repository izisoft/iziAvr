#include <core/kernel/iziKernelPriv.h>

#include "iziTaskPriv.h"

void iziTaskDelay(IziDelay_t waitTicks)
{
	IZI_ATOMIC_BLOCK()
	{
		gIziCurrentTask->_wakeUpTick = gIziTime + waitTicks;
		iziTaskWait();
	}
	iziKernelYeld();
}
