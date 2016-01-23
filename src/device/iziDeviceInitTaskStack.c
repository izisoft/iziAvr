#include <string.h>

#include <core/task/iziTaskPriv.h>

#include "iziDevicePriv.h"

#ifndef IZI_AVR_UNIT_TEST

void iziDeviceInitTaskStack(TIziTask *task, void (*taskFunction)(void))
{
	*(task->_stackPointer) = (uint8_t)((uint16_t)(iziTaskFinalize) & 0x00FF);
	*(task->_stackPointer-1) = (uint8_t)(((uint16_t)(iziTaskFinalize) >> 8) & 0x00FF);
	*(task->_stackPointer-2) = (uint8_t)((uint16_t)(taskFunction) & 0x00FF);
	*(task->_stackPointer-3) = (uint8_t)(((uint16_t)(taskFunction) >> 8) & 0x00FF);
	*(task->_stackPointer-4) = (uint8_t)(0x00); // r0
	*(task->_stackPointer-5) = (uint8_t)(IZI_SREG_INIT_VALUE);	// status register
	task->_stackPointer -= 37;
	memset(task->_stackPointer + 1, 0, 31);
}
#endif
