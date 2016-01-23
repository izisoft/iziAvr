#ifndef IZIKERNEL_H_
#define IZIKERNEL_H_

#include <iziAvr.h>
#include <iziConf.h>
#include <core/iziTask.h>
#include <utils/iziPreprocessor.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IZI_DEVICE_SIGNATURE IZI_PP_CAT3(SIGNATURE_0,SIGNATURE_1,SIGNATURE_2)
#define IZI_COMPILATION_HASH IZI_PP_UCAT4(IZI_DEVICE_SIGNATURE,IZI_CPU_FREQ,IZI_PP_CONSUME(IZI_SYSTEM_TICK_SOURCE),IZI_SYSTEM_TICK_RATE)

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
	eIziMallocError,
	eIziStackOverflow,
};
typedef enum EIziStateFlagDef EIziStateFlag;

void iziKernelYeld();

IziBool_t iziKernelCheckState(EIziStateFlag flag);

void iziKernelSetState(EIziStateFlag flag);

TIziTask* iziKernelCurrentTask();

void iziKernelStart();

#ifdef __cplusplus
}
#endif

#endif /* IZIKERNEL_H_ */
