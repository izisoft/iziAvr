#include <core/kernel/iziKernelPriv.h>
#include <device/iziDevicePriv.h>
#include "iziTaskPriv.h"

void iziTaskDelaySec(IziDelay_t waitSeconds)
{
	IZI_ATOMIC_BLOCK()
	{
		gIziCurrentTask->_wakeUpTick = gIziTime + ((uint32_t)(waitSeconds) * 2 * IZI_SYSTEM_TICK_RATE + 1) / 2;
		iziTaskWait();
	}
	iziKernelYeld();
}
