#ifndef IZI_FPM_H_
#define IZI_FPM_H_

#include <izi/avr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup iziUtils
 * @{
 */

/** \file iziFpm.h
 */

/**
 * \typedef TIziFpm
 * Type holding iziavr library specific
 * fixed point variable
 */
typedef int32_t	TIziFpm;

/** \fn iziFpmAdd(TIziFpm a, TIziFpm b) 	*/
TIziFpm iziFpmAdd(TIziFpm a, TIziFpm b);
/** \fn iziFpmAddi(TIziFpm a, int32_t b)	*/
TIziFpm iziFpmAddi(TIziFpm a, int32_t b);
/** \fn iziFpmAddf(TIziFpm a, double b) 	*/
TIziFpm iziFpmAddf(TIziFpm a, double b);

/** \fn iziFpmSub(TIziFpm a, TIziFpm b) 	*/
TIziFpm iziFpmSub(TIziFpm a, TIziFpm b);
/** \fn iziFpmSubi(TIziFpm a, int32_t b) 	*/
TIziFpm iziFpmSubi(TIziFpm a, int32_t b);
/** \fn iziFpmSubf(TIziFpm a, double b) 	*/
TIziFpm iziFpmSubf(TIziFpm a, double b);

/** \fn iziFpmMul(TIziFpm a, TIziFpm b) 	*/
TIziFpm iziFpmMul(TIziFpm a, TIziFpm b);
/** \fn iziFpmMuli(TIziFpm a, int32_t b) 	*/
TIziFpm iziFpmMuli(TIziFpm a, int32_t b);
/** \fn iziFpmMulf(TIziFpm a, double b) 	*/
TIziFpm iziFpmMulf(TIziFpm a, double b);

/** \fn iziFpmDiv(TIziFpm a, TIziFpm b) 	*/
TIziFpm iziFpmDiv(TIziFpm a, TIziFpm b);
/** \fn iziFpmDivi(TIziFpm a, int32_t b) 	*/
TIziFpm iziFpmDivi(TIziFpm a, int32_t b);
/** \fn iziFpmDivf(TIziFpm a, double b) 	*/
TIziFpm iziFpmDivf(TIziFpm a, double b);

/** \fn iziFpmFromFloat(double a) 	*/
TIziFpm iziFpmFromFloat(double a);
/** \fn iziFpmFromInt(int32_t a) 	*/
TIziFpm iziFpmFromInt(int32_t a);

/** \fn iziFpmToFloat(TIziFpm a) 	*/
double iziFpmToFloat(TIziFpm a);
/** \fn iziFpmToInt(TIziFpm a) 		*/
int32_t iziFpmToInt(TIziFpm a);

/// @}

#ifdef __cplusplus
}
#endif

#endif
