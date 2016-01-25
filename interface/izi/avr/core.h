#ifndef IZI_CORE_H_
#define IZI_CORE_H_

#include <izi/avr/config.h>
#include <izi/avr/types.h>
#include <izi/avr/core/kernel.h>
#include <izi/avr/core/task.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! Callback to be defined by client. Called by main
 * function defined inside the library. This function
 * should contain creation of at least one user task
 * followed by call to iziKernelStart().
 */
int iziMain();

/*! Callback to be optionally defined by client.
 * Function will be executed on every pass through
 * kernel task function loop.
 */
void iziKernelIdleHook();

/*! Callback to be optionally defined by client.
 * Function will be executed only once in the very
 * beginning of kernel task function execution.
 */
void iziKernelStartupHook();

#ifdef __cplusplus
}
#endif

#endif /* IZI_CORE_H_ */
