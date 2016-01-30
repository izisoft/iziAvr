#ifndef IZI_KERNELPRIV_H_
#define IZI_KERNELPRIV_H_

#include <izi/avr/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \def IZI_SET_INIT
 * Thread safe mutator for iziInitFlag variable.
 */
#define IZI_SET_STATE(flag)             IZI_ATOMIC_BLOCK() { gIziState |= _BV(flag); }

/*! \def IZI_GET_INIT
 * Getter for iziInitFlag variable.
 */
#define IZI_GET_STATE(flag)             ((gIziState & _BV(flag)) != 0)

#define IZI_FATAL(flag)					cli();                   \
										gIziState |= _BV(flag);  \
										for(;;)

#if IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_HPF
	typedef TIziTaskList TIziActiveTaskList;
#elif IZI_KERNEL_SCHEDULER == IZI_KERNEL_SCHEDULER_FTS
	typedef TIziScoredTaskList TIziActiveTaskList;
#endif

extern volatile TIziTask* gIziCurrentTask;
extern volatile TIziTask* gIziKernelTask;
extern volatile TIziActiveTaskList gIziActiveTaskList[IZI_TASK_PRIORITY_COUNT];
extern volatile TIziTaskList gIziWaitingTaskList;
extern volatile TIziTaskList gIziSuspendedTaskList;
extern volatile TIziTaskList gIziRemovedTaskList;
extern volatile uint32_t gIziTime;
extern volatile uint16_t gIziState;

#if IZI_KERNEL_TYPE == IZI_KERNEL_DEBUG
extern void iziKernelDebugYeldHook();
#endif

#ifdef __cplusplus
}
#endif

#endif /* IZI_KERNELPRIV_H_ */
