#ifndef IZI_TASK_H_
#define IZI_TASK_H_

/*!
 *	\addtogroup core
 *	@{
 *
 *	\file task.h
 */

#include <izi/avr/types.h>
#include <izi/avr/config.h>

#ifdef __cplusplus
extern "C" {
#endif

//! List of available task priorities.
enum EIziTaskPriorityDef
{
	eIziPrioLow = 0,                           //!< Lowest possible task priority. Used by kernel task.
	eIziPrioMedium = IZI_PRIORITY_MEDIUM_VALUE,//!< Medium task priority.
	eIziPrioHigh = IZI_PRIORITY_HIGH_VALUE,    //!< High task priority.
	eIziPrioRT,                                //!< Highest possible task priority, should be used for most important tasks with RT operations.
	IZI_PRIORITY_COUNT                         //!< Number of available priorities, used by scheduler to navigate on task queues.
};

typedef enum EIziTaskPriorityDef EIziTaskPriority;

typedef struct SIziTask TIziTask;
typedef struct SIziTaskList TIziTaskList;

struct SIziTaskList
{
	TIziTask* _iter;
};

/*!
 *
 */
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

/*! Every second contains exactly IZI_SYSTEM_TICK_RATE ticks. Calling
 * this function will suspend execution of caller until specified tick
 * in second appears. If current system tick is already higher than tick
 * specified, task will wake up within next second.
 * \param tick
 * Specific tick on which task should be woken up.
 */
void iziTaskSubscribeTick(IziTick_t tick);

/*! Suspends execution of caller for at least specified number of ticks.
 * \param waitTicks
 * Number of tick for which task should be suspended
 */
void iziTaskDelay(IziDelay_t waitTicks);

/*! Suspends execution of caller for specified amount of milliseconds.
 * Accuracy of this method depends on IZI_SYSTEM_TICK_RATE definition
 * and system clock frequency.
 * \param waitMs
 * Number of milliseconds for which task should be suspended
 */
void iziTaskDelayMs(IziDelay_t waitMs);

/*! Suspends execution of caller for specified amount of seconds.
 * Accuracy of this method depends on IZI_SYSTEM_TICK_RATE definition
 * and system clock frequency.
 * \param waitSeconds
 * Number of seconds for which task should be suspended
 */
void iziTaskDelaySec(IziDelay_t waitSeconds);

//! Provides access to parameters of current task.
void* iziTaskGetParams();

/*! Kill given task. Task execution is not interrupted instantly. Instead
 * it is moved out of active task list, thus it's not taken into consideration
 * during next context switch performed by scheduler. Task memory is freed
 * during cleanup, performed by system task.
 *
 * \param task
 * Pointer to task that should be killed.
 */
void iziTaskKill(TIziTask* task) ;

#ifdef __cplusplus
}
#endif

/// @}

#endif /* IZI_TASK_H_ */
