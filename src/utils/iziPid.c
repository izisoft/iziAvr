//#pre-defines
#define DBGU_SGN 	"PID"
#define DBGU_FLAG	FEAT_DBG_PID

#include <izi/avr/core/memory.h>
#include <izi/avr/core/kernel.h>
#include <izi/avr/utils/pid.h>

#define IZI_PID_STACK_SIZE	96

#define IZI_LIMIT(VAL,LOW,HIGH)      \
{                                    \
	if((VAL) < (LOW)) {              \
		VAL = LOW;                   \
	} else if((VAL)>(HIGH)) {        \
		VAL = HIGH;                  \
	}                                \
}

void iziPidTask()
{
	TIziPidController *me = (TIziPidController*)(iziKernelCurrentTask()->_params);
	int16_t error = 0;
	int16_t lastError = 0;
	int32_t sumError = 0;
	int32_t retVal;
	
	while(1)
	{
		error =  (me->setPoint) - (me->getValue());
		
		retVal = me->Kp * error;
		retVal += me->Kd * (error - lastError);
		sumError += error;
		IZI_LIMIT(sumError,-(me->sumLimit),me->sumLimit);
		retVal += me->Ki * sumError;
		retVal /= me->scalingFactor;
		
		IZI_LIMIT(retVal,-(me->retLimit),me->retLimit);
		me->setOutput((int16_t)(retVal));
		
		lastError = error;
		
		iziTaskDelay(5);
	}
}

TIziPidController *iziPidCreate(uint16_t (*getValue)(), void (*setOutput)(uint16_t),
	uint8_t Kp, uint8_t Ki, uint8_t Kd, uint8_t scalingFactor)
{
	TIziPidController *pidController;
	
	pidController = iziMalloc(sizeof(TIziPidController));
	pidController->pidTask = iziTaskCreate(iziPidTask,eIziPrioRT,IZI_PID_STACK_SIZE, NULL);
	pidController->pidTask->_params = (void*)(pidController);
	
	pidController->Kp = Kp;
	pidController->Ki = Ki;
	pidController->Kd = Kd;
	pidController->scalingFactor = scalingFactor;
	pidController->sumLimit = 2048;
	pidController->sumLimit = 512;

	return pidController;
}

void iziPidSetPoint(uint16_t setPoint,TIziPidController *pid)
{

}
