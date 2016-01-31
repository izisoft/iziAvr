#ifndef IZI_MEMORY_H_
#define IZI_MEMORY_H_

#include <stdlib.h>

#include <izi/avr/types.h>
#include <izi/avr/config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \addtogroup memory
 * @{
 *
 * \file memory.h
 * Part of iziAvr \ref core module. Provides simple preemption safe facade
 * over malloc and free functions.
 *
 * \note When writing multi-task applications using iziAvr library, you should
 * never use native memory access functions.
 */

/*! Allocates heap memory block of given size. If allocation
 * fails system execution is suspended and flag \ref eIziMallocError
 * in \ref gIziState is set.
 *
 * \param size
 * Desired size of memory block to be allocated (in bytes).
 * \return
 * Pointer to newly allocated memory.
 */
void* iziMalloc(size_t size);

/*! Frees given memory block.
 *
 * \param ptr
 * Pointer to memory block to be released.
 */
void iziFree(void* ptr);

#if IZI_KERNEL_TYPE > IZI_KERNEL_TINY

/*! Depending on the kernel type this function returns total number of bytes
 * currently allocated on heap using \ref iziMalloc function.
 *
 * \return
 * Number of heap memory bytes that are currently in use.
 */
IziSize_t iziGetUsedMemory();

#else

//! Special variant of memory usage accessor for tiny kernel types.
#define iziGetUsedMemory() (0)

#endif

//! @}

#ifdef __cplusplus
}
#endif

#endif /* IZI_MEMORY_H_ */
