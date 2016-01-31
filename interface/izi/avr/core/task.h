#ifndef IZI_TASK_H_
#define IZI_TASK_H_

#include <izi/avr/types.h>
#include <izi/avr/config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup task
 * @{
 *
 * \file task.h
 * Implementation of task that can be executed and scheduled with
 * iziAvr kernel. Task is an entity having it's own stack memory
 * area. It allows kernel to implement preemption mechanism and
 * temporary interrupt any task.
 */

#if IZI_KERNEL_TYPE == IZI_KERNEL_TINY
	#define IZI_PRIORITY_MEDIUM_VALUE 0
	#define IZI_PRIORITY_HIGH_VALUE   0
#else
	#define IZI_PRIORITY_MEDIUM_VALUE 1
	#define IZI_PRIORITY_HIGH_VALUE   2
#endif

//! List of available task priorities.
enum EIziTaskPriorityDef
{
	eIziTaskPrioLow = 0,                            //!< Lowest possible task priority. Used by kernel task.
	eIziTaskPrioMedium = IZI_PRIORITY_MEDIUM_VALUE, //!< Medium task priority.
	eIziTaskPrioHigh = IZI_PRIORITY_HIGH_VALUE,     //!< High task priority.
	eIziTaskPrioRT,                                 //!< Highest possible task priority, should be used for most important tasks with RT operations.
	IZI_TASK_PRIORITY_COUNT                         //!< Number of available priorities, used by scheduler to navigate on task queues.
};

/*! \typedef EIziTaskPriority
 * Type defined over task priority enumeration for convenience.
 */
typedef enum EIziTaskPriorityDef EIziTaskPriority;

typedef struct SIziTask TIziTask;
typedef struct SIziTaskList TIziTaskList;

/*!
 *
 */
struct SIziTaskList
{
	TIziTask* _iter;
};

/*! \struct SIziTask
 * Structure holding task data
 */
struct SIziTask
{
	uint8_t* _stackPointer; //!< Pointer to top of the task stack

	TIziTaskList* _owner;   //!< Pointer to task list holding task
	TIziTask* _next;        //!< Next task on the owning list
	TIziTask* _prev;        //!< Previous task on the owning list

	TIziTaskList* _eowner;  //!< Pointer to event list owning the task
	TIziTask* _enext;       //!< Next task on owning event list

	uint8_t _priority;      //!< Task priority
	uint32_t _wakeUpTick;   //!< Scheduled wake up time for the task

#if IZI_KERNEL_TYPE > IZI_KERNEL_TINY
	void* _params;          //!< Pointer to additional parameters provided by user
#endif

#if IZI_KERNEL_TYPE == IZI_KERNEL_DEBUG
	IziSize_t _stackSize;       //!< Initial size of task stack
	uint32_t _yeldCount;        //!< Number of yelds that caused activation of task
	uint8_t _stackUtilization;  //!< Usage of stack (0 is 0%, 255 is 100%)
	uint8_t _cpuUtilization;    //!< Usage of CPU (0 is 0%, 255 is 100%)
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

//! @}

#endif /* IZI_TASK_H_ */
