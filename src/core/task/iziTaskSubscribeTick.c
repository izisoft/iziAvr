#include <core/kernel/iziKernelPriv.h>

#include "iziTaskPriv.h"

void iziTaskSubscribeTick(IziTick_t tick)
{
	IZI_ATOMIC_BLOCK()
	{
		IziTick_t currentTick = (gIziTime % IZI_SYSTEM_TICK_RATE);
		if(tick > currentTick)
		{
			gIziCurrentTask->_wakeUpTick = gIziTime + (IziTick_t)(tick - currentTick);
		}
		else
		{
			gIziCurrentTask->_wakeUpTick = gIziTime + (IziTick_t)(IZI_SYSTEM_TICK_RATE + tick - currentTick);
		}
		iziTaskWait();
	}
	iziKernelYeld();
}
