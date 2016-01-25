#include <izi/avr/core/mutex.h>

void iziMutexCreate(TIziMutex* mutex)
{
	mutex->_value = 1;
	mutex->_owner = NULL;
	mutex->_subscribers._iter = NULL; //TODO check if _iter needs initialization when _taskCount is 0
}
