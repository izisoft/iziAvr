#include <izi/avr/core/kernel.h>
#include <device/iziDevicePriv.h>

#include <izi/avr/utils/preprocessor.h>

extern int iziMain();

int main()
{
	int tmpVal=0;

	IZI_INIT_MEM_MANAGER();

	tmpVal = iziMain();

#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_STANDARD
	// post mortem
#endif
	for(;;);
	return 0;
}
