#ifndef IZITWI_H_
#define IZITWI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum EIziTwiState
{
	eIziTwiIdle = 0,
	eIziTwiBusy,
	eIziTwiMT_Ok,
	eIziTwiMR_Ok,
	eIziTwiError,
	eIziTwiMT_Error,
	eIziTwiMR_Error
};
typedef enum EIziTwiState TIziTwiState;

void iziTwiInit(uint8_t bitrateKHz);

TIziTwiState iziTwiMasterSend(uint8_t addr, uint8_t *data, uint8_t dataLength);

TIziTwiState iziTwiMasterReceive(uint8_t addr, uint8_t *data, uint8_t dataLength);

TIziTwiState iziTwiMasterSendReceive(uint8_t addr,
		uint8_t *sendBuffer, uint8_t sendSize,
		uint8_t *recBuffer, uint8_t recSize);
		
#ifdef __cplusplus
}
#endif

#endif /* IZITWI_H_ */
