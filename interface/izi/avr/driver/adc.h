#ifndef IZI_ADC_H_ 
#define IZI_ADC_H_


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup driver
 * @{
 *
 * \file adc.h
 * iziAdc module handles all operations with AD converter
 * situated on AVT ATMega devices.
 */

/** \enum EIziAdcReference
 *
 ** \typedef TIziAdcRef
 *  Type for possible reference voltage source in AVR.
 */
enum EIziAdcReferenceDef
{
	eIziAdcRefAref,   		//!< Reference voltage connected to AREF pin.
	eIziAdcRefAvcc,	  		//!< Supply voltage is considered as reference.
	eIziAdcRefInternal=3 	//!< Turns on internal 2.56V reference source.
};

typedef enum EIziAdcReferenceDef EIziAdcReference;

/** \enum EIziAdcPrescaler
 *
 ** \typedef TIziAdcPrescaler
 *  Type for possible clock prescaler in AD converter.
 */
enum EIziAdcPrescalerDef
{
	eIziAdcClkDiv2=1,	//!< Clock divided by 2
	eIziAdcClkDiv4,		//!< Clock divided by 4
	eIziAdcClkDiv8,		//!< Clock divided by 8
	eIziAdcClkDiv16,	//!< Clock divided by 16
	eIziAdcClkDiv32,	//!< Clock divided by 32
	eIziAdcClkDiv64,	//!< Clock divided by 64
	eIziAdcClkDiv128	//!< Clock divided by 128
};

typedef enum EIziAdcPrescalerDef EIziAdcPrescaler;

/** \enum EIziAdcChannel
 *
 ** \typedef TIziAdcChannel
 *  Type for conversion channel to be measured.
 */
enum EIziAdcChannelDef
{
	eIziAdcCh0,
	eIziAdcCh1,
	eIziAdcCh2,
	eIziAdcCh3,
	eIziAdcCh4,
	eIziAdcCh5,
	eIziAdcCh6,
	eIziAdcCh7,
	eIziAdcCh0_Ch0_G10,
	eIziAdcCh1_Ch0_G10,
	eIziAdcCh0_Ch0_G200,
	eIziAdcCh1_Ch0_G200,
	eIziAdcCh2_Ch2_G10,
	eIziAdcCh3_Ch2_G10,
	eIziAdcCh2_Ch2_G200,
	eIziAdcCh3_Ch2_G200,
	eIziAdcCh0_Ch1,
	eIziAdcCh1_Ch1,
	eIziAdcCh2_Ch1,
	eIziAdcCh3_Ch1,
	eIziAdcCh4_Ch1,
	eIziAdcCh5_Ch1,
	eIziAdcCh6_Ch1,
	eIziAdcCh7_Ch1,
	eIziAdcCh0_Ch2,
	eIziAdcCh1_Ch2,
	eIziAdcCh2_Ch2,
	eIziAdcCh3_Ch2,
	eIziAdcCh4_Ch2,
	eIziAdcCh5_Ch2
};

typedef enum EIziAdcChannelDef EIziAdcChannel;

/** Configuration of AD converter. It must be executed before first usage of ADC.
 *  \param prescaler
 *  Prescaler for system clock.
 *  \param reference
 *  Reference voltage source.
 */
void iziAdcConfigure(EIziAdcPrescaler prescaler, EIziAdcReference reference);

/** \fn iziAdcTurnOn()
 *	Function turns on AD converter.
 */
void iziAdcEnable();

/** \fn iziAdcTurnOff()
 *	Function turns off AD converter.
 */
void iziAdcDisable();

/** Function executes AD conversion for chosen channel.
 *	Code is safe to use under iziKernel control. ADC unit is
 *	protected by iziMutex, and ongoing conversion doesn't take
 *	kernel time (synchronized by iziSemaphore).
 *
 *	\param channel
 *	AD channel to be read.
 *	\return
 *	Conversion result.
 */
uint16_t iziAdcReadChannel(EIziAdcChannel channel);

/// @}

#ifdef __cplusplus
}
#endif

#endif
