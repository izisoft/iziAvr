#ifndef IZICONF_H_
#define IZICONF_H_

#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif

//=================================================================================================
// CPU frequency
//---------------------------------------------------------------------------------------------
#if !defined(F_CPU)
#error "CPU frequency not defined"
#endif

/*! \def IZI_CPU_FREQ
 *	If cpu frequency is not defined in makefile, value provided by F_CPU is
 *	taken as standard frequency for iziRTOS.
 */
#define IZI_CPU_FREQ F_CPU

//=================================================================================================
// System tick source
//---------------------------------------------------------------------------------------------
#define IZI_SYSTEM_TICK_TC0        (0)
#define IZI_SYSTEM_TICK_TC1A       (1)
#define IZI_SYSTEM_TICK_TC1B       (2)
#define IZI_SYSTEM_TICK_TC2        (3)
#define IZI_SYSTEM_TICK_TC2_ASYNC  (4)

#ifndef IZI_SYSTEM_TICK_SOURCE
	#define IZI_SYSTEM_TICK_SOURCE IZI_SYSTEM_TICK_TC2_ASYNC
#endif

#ifndef IZI_SYSTEM_TICK_RATE
	#define IZI_SYSTEM_TICK_RATE 100
#endif

//=================================================================================================
// Scheduler type
//---------------------------------------------------------------------------------------------
#define IZI_KERNEL_SCHEDULER_HPF (0)
#define IZI_KERNEL_SCHEDULER_FTS (1)

#ifndef IZI_KERNEL_SCHEDULER
	#define IZI_KERNEL_SCHEDULER IZI_KERNEL_SCHEDULER_HPF
#endif

//=================================================================================================
// Kernel size
//---------------------------------------------------------------------------------------------
#define IZI_KERNEL_SIZE_TINY     (0)
#define IZI_KERNEL_SIZE_STANDARD (1)

#ifndef IZI_KERNEL_SIZE
	#define IZI_KERNEL_SIZE IZI_KERNEL_SIZE_STANDARD
#endif

//=================================================================================================
// Definitions specifiv for different kernel size
//---------------------------------------------------------------------------------------------
#if IZI_KERNEL_SIZE == IZI_KERNEL_SIZE_TINY

	#define IZI_PRIORITY_MEDIUM_VALUE (0)
	#define IZI_PRIORITY_HIGH_VALUE   (0)
	#undef IZI_MEMORY_MONITOR

//---------------------------------------------------------------------------------------------
#elif IZI_KERNEL_SIZE == IZI_KERNEL_SIZE_STANDARD
	#define IZI_PRIORITY_MEDIUM_VALUE (1)
	#define IZI_PRIORITY_HIGH_VALUE   (2)
	#define IZI_MEMORY_MONITOR        (1)
#endif

#ifdef __cplusplus
}
#endif

#endif /* IZICONF_H_ */
