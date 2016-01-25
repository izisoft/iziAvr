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

/*! Creates new task for set of given parameters.
 *
 * \param callback
 * Entry point for task. Note that multiple tasks can share the same entry point.
 * \param priority
 * Task priority.
 * \param stackSize
 * Size of the stack to be preallocated for task. This number should be carefully
 * estimated based on number of stack memory used by callback function.
 * \param param
 * Pointer to additional parameters that can be accessed from the callback
 * function.
 * \return
 * Pointer to newly allocated TIziTask structure.
 */
TIziTask* iziTaskCreate(void (*callback)(void), EIziTaskPriority priority,
	IziSize_t stackSize, void* param);

/** \fn iziTaskSubscribeTick( IziTick_t tick)
 * Every second contains exactly IZI_SYSTEM_TICK_RATE ticks. Calling
 * this function will suspend execution of caller until specified tick
 * in second appears. If current system tick is already higher than tick
 * specified, task will wake up within next second. *
 */
void iziTaskSubscribeTick(IziTick_t tick);

/** \fn iziTaskDelay( IziDelay_t waitTicks )
 * Suspends execution of caller for at least specified number of ticks.
 */
void iziTaskDelay(IziDelay_t waitTicks);

/** \fn iziTaskDelayMs( IziDelay_t waitTicks )
 * Suspends execution of caller for specified amount of milliseconds.
 * Accuracy of this method depends on IZI_SYSTEM_TICK_RATE definition
 * and system clock frequency.
 */
void iziTaskDelayMs(IziDelay_t waitMs);

/** \fn iziTaskDelaySec( IziDelay_t waitSeconds )
 * Suspends execution of caller for specified amount of seconds.
 * Accuracy of this method depends on IZI_SYSTEM_TICK_RATE definition
 * and system clock frequency.
 */
void iziTaskDelaySec(IziDelay_t waitSeconds);

void* iziTaskGetParams();

#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_TINY

/** \fn iziTaskKill(TIziTask* task)
 * Functionality not present in tiny kernel model.
 */
void iziTaskKill( TIziTask* task) ;

#endif

#ifdef __cplusplus
}
#endif

#endif /* IZITASK_H_ */
