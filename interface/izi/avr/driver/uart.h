#ifndef IZI_UART_H_
#define IZI_UART_H_
	
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup driver
 * @{
 */

/** \file uart.h
*/

/** \enum EIziUartInterface
 *
 ** \typedef TIziUartInterface
 *
 */
enum EIziUartInterface {
	eIziUartInterface_0,
	eIziUartInterfaceCount
};
typedef enum EIziUartInterface TIziUartInterface;

/** \fn iziUartConfigureUnit(TIziUartInterface interface, uint32_t baudrate)
 * Function configures chosen UART interface. On supported Avr platforms only
 * one interface is provided.
 * \param interface
 * Identifier of interface to be configured.
 * \param baudrate
 * Chosen baudrate for interface.
 */
void iziUartConfigureUnit(TIziUartInterface interface, uint32_t baudrate);

/** \fn iziUartWriteStream(TIziUartInterface interface, const char *data, uint8_t dataCount)
 * Function used to write stream through chosen UART interface.
 * \param interface
 * Identifier of interface on which transmission will be performed.
 * \param data
 * Pointer to provided data buffer.
 * \param dataCount
 * Number of bytes to be send.
 */
uint8_t iziUartWriteStream(TIziUartInterface interface, const char *data, uint8_t dataCount);

/** \fn iziUartReadStream(TIziUartInterface interface, char *data, uint8_t dataCount)
 * Function used to read from stream through chosen UART interface.
 * \param interface
 * Identifier of interface from which data will be received.
 * \param data
 * Pointer to provided data buffer.
 * \param dataCount
 * Number of bytes to be received.
 */
uint8_t iziUartReadStream(TIziUartInterface interface, char *data, uint8_t dataCount);

/** \fn iziUartGetchar(TIziUartInterface interface)
 * Function used to collect single byte from selected UART interface.
 * \param interface
 * Identifier of interface from which data will be received.
 * \return
 * Function returns read character.
 */
char iziUartGetchar(TIziUartInterface interface);

/** \fn iziUartPutchar(TIziUartInterface interface,char c)
 * Function used to put single byte to selected UART interface.
 * \param interface
 * Identifier of interface where byte will be send.
 * \param c
 * Character to be send through interface.
 */
void iziUartPutchar(TIziUartInterface interface,char c);

/// @}

#ifdef __cplusplus
}
#endif

#endif //UART_H
