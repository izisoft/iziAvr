#ifndef IZI_CRC_H
#define IZI_CRC_H

#include <izi/avr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IZI_CRC_16

typedef uint16_t IziCrc_t;

#else

typedef uint8_t IziCrc_t;

#endif

/**
 *	\addtogroup iziUtils 
 *	@{
 */
	
/**
 * \file iziCrc.h
 */

/** \fn iziCrcCalculate(const char *data,size_t dataSize)
 * \param data
 * \param dataSize
 * \return
 */
IziCrc_t iziCrcCalculate(const char *data,size_t dataSize);
 
/** \fn iziCrc8(uint8_t crc, uint8_t data)
 * \param crc
 * \param data
 * \return
 */
uint8_t iziCrc8(uint8_t crc, uint8_t data);

/** \fn iziCrc16(uint16_t* crc, uint8_t data)
 * \param crc
 * \param data
 * \return
 */
uint16_t iziCrc16(uint16_t* crc, uint8_t data);

/// @}

#ifdef __cplusplus
}
#endif

#endif
