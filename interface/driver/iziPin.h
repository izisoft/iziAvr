#ifndef IZIPIN_H_
#define IZIPIN_H_

#include <iziAvr.h>

#include "iziPort.h"

#ifdef __cplusplus
extern "C" {
#endif

enum EIziPinDef
{
	eIziPinA0 = (eIziPortA << 8),
	eIziPinA1,
	eIziPinA2,
	eIziPinA3,
	eIziPinA4,
	eIziPinA5,
	eIziPinA6,
	eIziPinA7,

	eIziPinB0 = (eIziPortB << 8),
	eIziPinB1,
	eIziPinB2,
	eIziPinB3,
	eIziPinB4,
	eIziPinB5,
	eIziPinB6,
	eIziPinB7,

	eIziPinC0 = (eIziPortC << 8),
	eIziPinC1,
	eIziPinC2,
	eIziPinC3,
	eIziPinC4,
	eIziPinC5,
	eIziPinC6,
	eIziPinC7,

	eIziPinD0 = (eIziPortD << 8),
	eIziPinD1,
	eIziPinD2,
	eIziPinD3,
	eIziPinD4,
	eIziPinD5,
	eIziPinD6,
	eIziPinD7,
};

typedef enum EIziPinDef EIziPin;

enum EIziPinTypeDef
{
	eIziPinIn,
	eIziPinOut
};
typedef enum EIziPinTypeDef EIziPinType;

void iziPinSetType(EIziPin pin, EIziPinType type);

void iziPinSet(EIziPin pin, IziBool_t value);

IziBool_t iziPinGet(EIziPin pin);

void iziPinToggle(EIziPin pin);

#ifdef __cplusplus
}
#endif

#endif /* IZIPIN_H_ */
