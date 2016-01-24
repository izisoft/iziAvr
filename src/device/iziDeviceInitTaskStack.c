#include <string.h>

#include <core/task/iziTaskPriv.h>

#include "iziDevicePriv.h"

void iziDeviceInitTaskStack(TIziTask* task, void (*taskFunction)(void))
{
	*(task->_stackPointer--) = (uint8_t)((uint16_t)(iziTaskFinalize) & 0x00FF);
	*(task->_stackPointer--) = (uint8_t)(((uint16_t)(iziTaskFinalize) >> 8) & 0x00FF);
	*(task->_stackPointer--) = (uint8_t)((uint16_t)(taskFunction) & 0x00FF);
	*(task->_stackPointer--) = (uint8_t)(((uint16_t)(taskFunction) >> 8) & 0x00FF);
	*(task->_stackPointer--) = (uint8_t)(0x00); // r0
	*(task->_stackPointer--) = (uint8_t)(IZI_SREG_INIT_VALUE);	// status register
	task->_stackPointer -= 31;
	memset(task->_stackPointer + 1, 0, 31);
}
