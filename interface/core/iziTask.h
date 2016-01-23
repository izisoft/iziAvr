#ifndef IZITASK_H_
#define IZITASK_H_

#include <iziAvr.h>
#include <iziConf.h>

#ifdef __cplusplus
extern "C" {
#endif

enum EIziTaskPriorityDef
{
	eIziPrioLow = 0,
	eIziPrioMedium = IZI_PRIORITY_MEDIUM_VALUE,
	eIziPrioHigh = IZI_PRIORITY_HIGH_VALUE,
	eIziPrioRT,
	IZI_PRIORITY_COUNT
};

typedef enum EIziTaskPriorityDef EIziTaskPriority;

typedef struct SIziTask TIziTask;
typedef struct SIziTaskList TIziTaskList;

struct SIziTaskList
{
	TIziTask* _iter;
};

struct SIziTask
{
	uint8_t* _stackPointer;

	//! Node for kernel task lists
	TIziTaskList* _owner;
	TIziTask* _next;
	TIziTask* _prev;

	//! Node for event task lists
	TIziTaskList* _eowner;
	TIziTask* _enext;

	uint8_t _priority;
	uint32_t _wakeUpTick;

#ifndef IZI_KERNEL_DEBUG
	IziSize_t _stackSize;
	uint32_t _yeldCount;
	uint8_t _stackUtilization;
	uint8_t _cpuUtilization;
#endif

#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_TINY
	void* _params;
#endif
};

TIziTask* iziTaskCreate(void (*callback)(void), EIziTaskPriority priority,
	IziSize_t stackSize, void* param);

/** \fn iziTaskSubscribeTick( uint8_t tick)
 *
 */
void iziTaskSubscribeTick(IziTick_t tick);

/** \fn iziTaskDelay( IziDelay_t waitTicks )
 *
 */
void iziTaskDelay(IziDelay_t waitTicks);

/** \fn iziTaskDelaySec( IziDelay_t waitSeconds )
 *
 */
void iziTaskDelaySec(IziDelay_t waitSeconds);

#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_TINY

/** \fn iziTaskDelete(TIziTask* task)
 * Functionaluy not present in tiny kernel model.
 */
void iziTaskKill( TIziTask* task) ;

#endif

#ifdef __cplusplus
}
#endif

#endif /* IZITASK_H_ */
