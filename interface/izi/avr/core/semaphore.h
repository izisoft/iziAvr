#ifndef IZI_SEMAPHORE_H_
#define IZI_SEMAPHORE_H_

#include <izi/avr/core/task.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *	\addtogroup iziKernel
 *	@{
 */

/** \file iziSemaphore.h
 * Simple implementation of binary semaphore functionality for iziKernel
 * RTOS. Counting semaphores will be supported in future versions of kernel
 * (v2.0 onwards).
 */

/** \typedef TIziSemaphore
 * Type of semaphore used in iziKernel RTOS.
 */
typedef volatile struct SIziSemaphore TIziSemaphore;

/** \struct SIziSemaphore
 * Structure holding implementation of iziKernel binary semaphore.
 */
struct SIziSemaphore
{
	int8_t _value; 				//!< Current semaphore state.
	TIziTaskList _subscribers;	//!< List of tasks that has subscribed semaphore.
};

/** \fn iziSemaphoreCreate(TIziSemaphore *semaphore,uint8_t isAvailable)
 * Function used to create semaphore for iziKernel. It must be called
 * before first use of semaphore!
 * \param semaphore
 * Pointer to semaphore to be created.
 * \param isAvailable
 * Value indicating if semaphore should be available to take after creation.
 * If IziFalse, semaphore wont be able to take before first call of
 * iziSemaphorGive or iziSemaphoreGiveFromIsr functions.
 */
void iziSemaphoreCreate(TIziSemaphore *semaphore,uint8_t isAvailable);

/** \fn iziSemaphoreTake(TIziSemaphore *semaphore,IziDelay_t waitTicks)
 * Function takes semaphore if available.
 * \param semaphore
 * Pointer to semaphore to be taken
 * \param waitTicks
 * Maximum number of ticks that task can wait for semaphore.
 * \return
 * IziTrue if semaphore was taken properly (immediately or before set timeout elapsed),
 * IziFalse otherwise.
 */
IziBool_t iziSemaphoreTake(TIziSemaphore *semaphore,IziDelay_t waitTicks);

/** \fn iziSemaphoreTaskeFromIsr(TIziSemaphore *semaphore)
 * Function takes semaphore if available from interrupt service routine.
 * \param semaphore
 * Pointer to semaphore to be taken.
 * \return
 * IziTrue if semaphore was taken properly, IziFalse otherwise.
 */
IziBool_t iziSemaphoreTaskeFromIsr(TIziSemaphore *semaphore);

/** \fn iziSemaphoreGive(TIziSemaphore *semaphore)
 * Function gives semaphore. After call semaphore is available to
 * be taken.
 * \param semaphore
 * Pointer to semaphore to be given
 * \return
 * IziTrue if semaphore was given properly, IziFalse otherwise
 * (e.g it was already available).
 */
IziBool_t iziSemaphoreGive(TIziSemaphore *semaphore);

/** \fn iziSemaphoreGiveFromIsr(TIziSemaphore *semaphore,uint8_t *needYeld)
 * Function gives semaphore if available from interrupt service routine.
 * \param semaphore
 * Pointer to semaphore to be given
 * \param needYeld
 * Pointer to uint8_t value which will be set by function and indicate need
 * of context switch (e.g when higher priority task was waiting for given
 * semaphore)
 * \return
 * IziTrue if semaphore was given properly, IziFalse otherwise
 * (e.g it was already available).
 */
IziBool_t iziSemaphoreGiveFromIsr(TIziSemaphore *semaphore,uint8_t *needYeld);

/// @}

#ifdef __cplusplus
}
#endif

#endif // IZI_SEMAPHORE_H_
