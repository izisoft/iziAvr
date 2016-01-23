#include <device/iziDevicePriv.h>
#include <driver/iziPort.h>

#define IZI_DDR_OFFSET (-0x01)
#define IZI_PIN_OFFSET (-0x02)

#define IZI_PORT(port) _SFR_IO8(port)
#define IZI_DDR(port) _SFR_IO8(port + IZI_DDR_OFFSET)
#define IZI_PIN(port) _SFR_IO8(port + IZI_PIN_OFFSET)

void iziPortSetType(EIziPort port, IziPortMask_t type)
{
	IZI_ATOMIC_INSTRUCTION(IZI_DDR(port) = type);
}

void iziPortSet(EIziPort port, IziPortMask_t value)
{
	IZI_ATOMIC_INSTRUCTION(IZI_PORT(port) = value);
}

IziPortMask_t iziPortGet(EIziPort port)
{
	IziPortMask_t value;
	IZI_ATOMIC_BLOCK()
	{
		value = IZI_PIN(port);
	}
	return value;
}

void iziPortToggle(EIziPort port)
{
	IZI_ATOMIC_INSTRUCTION(IZI_PORT(port) = ~IZI_PORT(port));
}
