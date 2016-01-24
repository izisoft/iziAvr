#include <core/kernel/iziKernelPriv.h>
#include "iziTaskPriv.h"

void* iziTaskGetParams(TIziTask* task)
{
	if(task != NULL)
	{
		return task->_params;
	}
	else
	{
		return gIziCurrentTask->_params;
	}
}
