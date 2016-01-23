#ifndef IZIEEPROM_H
#define IZIEEPROM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup iziPlatform
 * @{
 */

typedef uint16_t IziEepromAddr_t;

/** \file iziEeprom.h
*/

enum EIziEepromMap {
	IZI_LAST_RESET_SOURCE = 0,
	IZI_TIME_BEFORE_RESET = 1,
	IZI_TASK_BEFORE_RESET = 5
};

/** 
 * \fn iziEepromInit( void )
 *	Initialization function for iziEeprom module. It must be called before
 *	very first eeprom read/write operation. Best place for calling iziEepromInit
 *	is iziKernelStartupHook() function.
 */
void iziEepromInit( void );

/**
 * \fn iziEepromWrite(uint16_t position, const char *data, uint8_t dataCount)
 *	\param position
 *	Address in user area in eeprom memory where data should be written.
 *	\param data
 *	Pointer to data, that should be written to eeprom memory.
 *	\param dataCount
 *	Number of bytes to be written to eeprom memory.
 *	\return
 *	Function returns 0 if writing to eeprom succeeds.
 */
uint8_t iziEepromWrite(IziEepromAddr_t position, const char *data, uint8_t dataCount);

/**
 * \fn iziEepromRead(uint16_t position, char *data, uint8_t dataCount)
 *	\param position
 *	Address in user area in eeprom memory with data to be read.
 *	\param data
 *	Pointer to data, that should be read from eeprom memory.
 *	\param dataCount
 *	Number of bytes to be read from eeprom memory.
 *	\return
 *	Function returns 0 if reading from eeprom succeeds.
 */
uint8_t iziEepromRead(IziEepromAddr_t position, char *data, uint8_t dataCount);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* IZIEEPROM_H */
