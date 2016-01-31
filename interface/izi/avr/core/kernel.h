/*! \page core iziAvr Core
 * \ref kernel
 *
 * \ref task
 *
 * \ref mutex
 *
 * \ref semaphore
 *
 * \ref queue
 *
 * \ref memory
 */
#ifndef IZI_KERNEL_H_
#define IZI_KERNEL_H_

#include <izi/avr/types.h>
#include <izi/avr/config.h>
#include <izi/avr/core/task.h>
#include <izi/avr/utils/preprocessor.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup kernel
 * @{
 * \file kernel.h
 */

#define IZI_DEVICE_SIGNATURE IZI_PP_CAT3(SIGNATURE_0,SIGNATURE_1,SIGNATURE_2)
#define IZI_COMPILATION_HASH IZI_PP_UCAT5(IZI_DEVICE_SIGNATURE,IZI_CPU_FREQ,\
								IZI_KERNEL_TYPE,IZI_SYSTEM_TICK_SOURCE,IZI_SYSTEM_TICK_RATE)

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

/*! Forces scheduler context switch.
 *
 * \note Calling this function doesn't mean that scheduler will peak up
 * different task for execution than the current one.
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

/*! Holds current state of the system. Each bits corresponds to
 * one of the \ref EIziStateFlagDef flags.
 */
extern volatile uint16_t gIziState;

#ifdef __cplusplus
}
#endif

//! @}

#endif /* IZI_KERNEL_H_ */
