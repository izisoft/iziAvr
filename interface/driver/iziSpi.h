#ifndef IZISPI_H_
#define IZISPI_H_

#include <iziAvr.h>
#include <driver/iziPin.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup iziPlatform
 * @{
 */

/** \file iziSpi.h
 *
 */

/** \enum EIziSpiWorkingMode
 *
 ** \typedef TIziSpiWorkingMode
 *
 */
enum EIziSpiWorkingMode {
	eIziSpiSlaveMode,	//!< eIziSpiSlaveMode
	eIziSpiMasterMode	//!< eIziSpiMasterMode
};
typedef enum EIziSpiWorkingMode	TIziSpiWorkingMode;

/** \enum EIziSpiDataOrded
 *
 ** \typedef TIziSpiDataOrder
 *
 */
enum EIziSpiDataOrded {
	eIziSpiMsb,			//!< eIziSpiMsb Most significant bit goes first.
	eIziSpiLsb 			//!< eIziSpiLsb Less significant bit goes first.
};
typedef enum EIziSpiDataOrded TIziSpiDataOrder;

/** \enum EIziSpiTrasmissionMode
 *	Mode of SPI transmission. Determinate clock phase and polarity.
 *
 ** \typedef TIziSpiTrasmissionMode
 *
 */
enum EIziSpiTrasmissionMode {
	eIziSpiMode0,		//!< eIziSpiMode0
	eIziSpiMode1,		//!< eIziSpiMode1
	eIziSpiMode2,		//!< eIziSpiMode2
	eIziSpiMode3 		//!< eIziSpiMode3
};
typedef enum EIziSpiTrasmissionMode TIziSpiTrasmissionMode;

/** \typedef TSpiUnit
*/
typedef struct {
	EIziPin pCsPin;
	uint8_t xSPCR;
	uint8_t xSPSR;	
} TIziSpiUnit;

/** \fn iziSpiAddUnit(const TIziPio* csPin, TIziSpiTrasmissionMode spiMode,uint16_t wantdeClock_kHz)
 * Function creates new unit for communication through SPI interface. Unit is identified by
 * chip select pin. There is no control preventing from creating two different units on
 * single pin, but it will probably lead to a crash.
 * \param csPin
 * \param spiMode
 * \param wantedClock_kHz
 * \return
 */
TIziSpiUnit* iziSpiAddUnit(EIziPin csPin,
						TIziSpiTrasmissionMode spiMode,
						uint16_t wantedClock_kHz);

/** \fn iziSpiSendStream(TIziSpiUnit* spiUnit, char* data, uint8_t dataCount)
 * Function used to send data stream via chosen SPI interface.
 * \param spiUnit
 * \param data
 * \param dataCount
 */
void iziSpiSendStream(TIziSpiUnit* spiUnit, char* data, uint8_t dataCount);

/** \fn iziSpiSelectUnit(TIziSpiUnit* pSpiUnit)
 * Function used to select chosen SPI interface for transmission.
 * \param spiUnit
 */
void iziSpiSelectUnit(TIziSpiUnit* spiUnit);

/** \fn iziSpiDeselectUnit(TIziSpiUnit* pSpiUnit)
 * Function used to deselect chosen SPI interface after transmission.
 * \param spiUnit
 */
void iziSpiDeselectUnit(TIziSpiUnit* spiUnit);

/// @}

#ifdef __cplusplus
}
#endif

#endif // IZISPI_H_
