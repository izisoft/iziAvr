#include "iziKernelPriv.h"

IziBool_t iziKernelCheckState(EIziStateFlag flag)
{
	return ((gIziState & _BV(flag)) != 0);
}
