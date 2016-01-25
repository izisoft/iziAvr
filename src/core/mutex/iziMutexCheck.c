#include <device/iziDevicePriv.h>
#include <izi/avr/core/mutex.h>

IziBool_t iziMutexCheck(TIziMutex* mutex)
{
	IziBool_t retVal = IziFalse;
	IZI_ATOMIC_BLOCK()
	{
		retVal = (mutex->_value == 1);
	}
	return retVal;
}
