#ifndef IZI_KERNEL_H_
#define IZI_KERNEL_H_

#include <izi/avr/types.h>
#include <izi/avr/config.h>
#include <izi/avr/core/task.h>
#include <izi/avr/utils/preprocessor.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IZI_DEVICE_SIGNATURE IZI_PP_CAT3(SIGNATURE_0,SIGNATURE_1,SIGNATURE_2)
#define IZI_COMPILATION_HASH IZI_PP_UCAT4(IZI_DEVICE_SIGNATURE,IZI_CPU_FREQ,IZI_SYSTEM_TICK_SOURCE,IZI_SYSTEM_TICK_RATE)

#define iziKernelStart IZI_PP_CAT(_iziKernelStart_, IZI_COMPILATION_HASH)

/** \enum EIziInitFlags
 *
 */
enum EIziStateFlagDef
{
	eIziKernelInit,		//!< Flag indicating that iziKernel scheduler is working
	eIziKernelStarted,	//!< Flag indicating that iziKernel scheduler is working
	eIziAdcInit,		//!< Flag indicating iziAdc module initialization
	eIziEepromInit,		//!< Flag indicating iziEeprom module initialization
	eIziSpiInit,		//!< Flag indicating iziSpi module initialization
	eIziTwiInit,		//!< Flag indicating iziTwi(iziI2C) module initialization
	eIziUartInit,		//!< Flag indicating iziUart module initialization
	eIziConsoleInit,	//!< Flag indicating iziConsole utility startup
	eIziStorageInit, 	//!< Flag indicating iziStorage utility startup
	eIziLoggerInit,		//!< Flag indicating iziLogger utility startup
	eIziMallocError,    //!< Flag indicating fatal error during memory allocation
	eIziStackOverflow,  //!< Flag indicating stack corruption detection
};
typedef enum EIziStateFlagDef EIziStateFlag;

/*!
 *
 */
void iziKernelYeld();

/*! Function providing access to currently executed task.
 *
 * \return
 * Pointer to task being currently executed.
 */
TIziTask* iziKernelCurrentTask();

/*! Starts kernel scheduler.
 *
 */
void iziKernelStart();

#ifdef __cplusplus
}
#endif

#endif /* IZI_KERNEL_H_ */
