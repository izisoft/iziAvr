#include "iziKernelPriv.h"

TIziTask* iziKernelCurrentTask()
{
	return (TIziTask*)gIziCurrentTask;
}
