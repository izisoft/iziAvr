#ifndef IZIRESET_H
#define IZIRESET_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup iziUtils
 * @{
 */
 
/** \file iziReset.h 
	Software reset functionality for iziavr library. Provides method
	to reset device.
*/

/** \enum EIziResetType
 * Enumeration of available reset types.
 *
 ** \typedef TIziResetType
 * Type used for recognizing requested reset type.
 */
enum EIziResetType{
	/** Unexpected reset caused by low level on external reset pin or software reset
	caused by non iziavr library code.*/
	eIziUnexpectedReset,
	/** After cold reset iziavr library state is completly cleared. System doesn't 
	care about unfinished tasks.*/
	eIziColdReset,
	/** Before executing warm reset library waits for all system guards, but no longer
	than 100ms. */
	eIziWarmReset,
	/** Recovery reset is similar to warm reset, but in addition all important system 
	variables are stored to EEProm memory.*/
	eIziRecoveryReset
};
typedef enum EIziResetType TIziResetType;

void iziReset(TIziResetType resetType);

/// @}

#ifdef __cplusplus
}
#endif

#endif
