#ifndef IZI_PORT_H_
#define IZI_PORT_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum EIziPortDef
{
	eIziPortA = 0x1B,
	eIziPortB = 0x18,
	eIziPortC = 0x15,
	eIziPortD = 0x12,
};

typedef enum EIziPortDef EIziPort;

typedef uint8_t IziPortMask_t;

void iziPortSetType(EIziPort port, IziPortMask_t type);

void iziPortSet(EIziPort port, IziPortMask_t value);

IziPortMask_t iziPortGet(EIziPort port);

void iziPortToggle(EIziPort pin);

#ifdef __cplusplus
}
#endif

#endif /* IZI_PORT_H_ */
