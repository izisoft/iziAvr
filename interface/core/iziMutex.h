#ifndef IZIMUTEX_H
#define IZIMUTEX_H

#include <core/iziTask.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup iziKernel
 * @{
 *
 * \file iziMutex.h
 * Implementation of simple mutex used in iziKernel RTOS.
 * iziMutex supports priority inheritance. Thanks that, if
 * task holding mutex has lower priority than task waiting for
 * it, mutex holder priority is raised temporarily to level of
 * waiting task (it prevents priority inversion problem).
 * Recursive mutexes will be supported in further versions of iziKernel.
 */

/** \struct SIziMutex
 * Structure holding mutex data.
 */
typedef struct SIziMutex
{
	int8_t _value;	//!< Current mutex state.
	TIziTask *_owner;	//!< Pointer to mutex owner task.
	EIziTaskPriority _ownerPriority;	//!< Original priority of mutex owner.
	TIziTaskList _subscribers;	//!< List of tasks that has subsrcibed mutex.
} TIziMutex;

/** \typedef TIziMutex
 * Type for mutex used in iziKernel RTOS.
 */
//typedef volatile struct SIziMutex TIziMutex;

/** \fn iziMutexCreate(TIziMutex *mutex)
 * Function creates and initializes mutex. It must be called before
 * very first usage of mutex.
 * \param mutex
 * Pointer to mutex to be initialized.
 */
void iziMutexCreate(TIziMutex *mutex);

/** \fn iziMutexTake(TIziMutex *mutex, IziDelay_t waitTicks)
 * Function attempts to take mutex if available.
 * \param mutex
 * Pointer to mutex to be taken.
 * \param waitTicks
 * Maximal time that task can wait for mutex to be released.
 * \return
 * IziTrue if mutex was taken successfully, IziFalse otherwise
 * (e.g. timeout expired)
 */
IziBool_t iziMutexTake(TIziMutex *mutex, IziDelay_t waitTicks);

/** \fn iziMutexGive(TIziMutex *mutex)
 * Function attempts to give mutex.
 * \param mutex
 * Pointer to mutex to be given.
 * \return
 * IziTrue if operation succeeded, IziFalse otherwise
 * (e.g. task calling function is not an owner of mutex).
 */
IziBool_t iziMutexGive(TIziMutex *mutex);

/** \fn iziMutexCheck(TIziMutex *mutex)
 * Function checks current status of iziMutex.
 * \return
 * IziTrue if mutex is free to be taken, IziFalse otherwise.
 */
IziBool_t iziMutexCheck(TIziMutex *mutex);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* IZIMUTEX_H_ */
