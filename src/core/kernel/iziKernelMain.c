#include <izi/avr/core/kernel.h>
#include <izi/avr/utils/preprocessor.h>

#include <device/iziDevicePriv.h>

extern int iziMain();

int main()
{
	int tmpVal=0;

	IZI_INIT_MEM_MANAGER();

	tmpVal = iziMain();

#if IZI_KERNEL_TYPE > IZI_KERNEL_STANDARD
	// post mortem
#endif
	for(;;);
	return 0;
}
