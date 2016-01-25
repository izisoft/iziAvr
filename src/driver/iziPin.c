#include <izi/avr/driver/pin.h>

#include <device/iziDevicePriv.h>

#define IZI_PORT_ID(pin) ((pin) & 0xFF00)
#define IZI_DDR_ID(pin) IZI_PORT_ID(pin - 0x0100)
#define IZI_PIN_ID(pin) IZI_PORT_ID(pin - 0x0200)

#define IZI_PORT(pin) _SFR_IO8(IZI_PORT_ID(pin) >> 8)
#define IZI_DDR(pin) _SFR_IO8(IZI_DDR_ID(pin) >> 8)
#define IZI_PIN(pin) _SFR_IO8(IZI_PIN_ID(pin) >> 8)


#define IZI_PIN_MASK(pin) _BV(pin & 0x07)

typedef volatile uint8_t* IziPort_t;

void iziPinSetType(EIziPin pin, EIziPinType type)
{
	if(type == eIziPinIn)
	{
		IZI_ATOMIC_INSTRUCTION(IZI_DDR(pin) &= ~IZI_PIN_MASK(pin));
	}
	else
	{
		IZI_ATOMIC_INSTRUCTION(IZI_DDR(pin) |= IZI_PIN_MASK(pin));
	}
}

void iziPinSet(EIziPin pin, IziBool_t value)
{
	if(value == IziTrue)
	{
		IZI_ATOMIC_INSTRUCTION(IZI_PORT(pin) |= IZI_PIN_MASK(pin));
	}
	else
	{
		IZI_ATOMIC_INSTRUCTION(IZI_PORT(pin) &= ~IZI_PIN_MASK(pin));
	}
}

IziBool_t iziPinGet(EIziPin pin)
{
	IziBool_t value;
	IZI_ATOMIC_BLOCK()
	{
		value = ((IZI_PIN(pin) & IZI_PIN_MASK(pin)) != 0);
	}
	return value;
}

void iziPinToggle(EIziPin pin)
{
	IZI_ATOMIC_INSTRUCTION(IZI_PORT(pin) ^= IZI_PIN_MASK(pin));
}
