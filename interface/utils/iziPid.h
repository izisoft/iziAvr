#ifndef IZIPID_H
#define IZIPID_H
	
#include <core/iziTask.h>
	
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup iziUtils
 * @{
 */

/** \file iziPid.h
	PID regulator utility for iziavr library
*/

typedef struct {
  //! Last process value, used to find derivative of process value.
  int16_t lastProcessValue;
  //! Summation of errors, used for integrate calculations
  int32_t sumError;
  //! The Proportional tuning constant, multiplied with SCALING_FACTOR
  int16_t P_Factor;
  //! The Integral tuning constant, multiplied with SCALING_FACTOR
  int16_t I_Factor;
  //! The Derivative tuning constant, multiplied with SCALING_FACTOR
  int16_t D_Factor;
  //! Maximum allowed error, avoid overflow
  int16_t maxError;
  //! Maximum allowed sumerror, avoid overflow
  int32_t maxSumError;
} TPidData;

typedef enum EIziPidOutput TIziPidOutput;
enum EIziPidOutput {
	eIziPidOutput0,
	eIziPidOutput1,
	eIziPidOutput2
};

typedef struct TIziPidGain TIziPidGain;
struct TIziPidGain {
	uint16_t Kp;
	uint16_t Ki;
	uint16_t Kd;
};

typedef struct SIziPidController TIziPidController;
struct SIziPidController {
	TIziTask *pidTask;
	uint8_t Kp;
	uint8_t Ki;
	uint8_t Kd;
	uint8_t scalingFactor;
	uint16_t sumLimit;
	uint16_t retLimit;
	int16_t setPoint;
	int16_t (*getValue)();
	void (*setOutput)(int16_t);
};
	
void iziPidInit(int16_t Kp, int16_t Ki, int16_t Kd, TPidData *pid);

int16_t iziPidCalculate(int16_t setPoint, int16_t processValue, TPidData *pid);

void iziPidReset(TPidData *pid);

/// @}

#ifdef __cplusplus
}
#endif

#endif
