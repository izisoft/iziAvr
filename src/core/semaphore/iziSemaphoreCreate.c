#include <izi/avr/core/semaphore.h>

void iziSemaphoreCreate(TIziSemaphore* semaphore, uint8_t isAvailable)
{
	semaphore->_subscribers._iter = NULL;
	semaphore->_value = (isAvailable == IziTrue) ? 1 : 0;
}
