#include "iziKernelPriv.h"

IziBool_t iziKernelCheckState(EIziStateFlag flag)
{
	return IZI_GET_STATE(flag);
}
