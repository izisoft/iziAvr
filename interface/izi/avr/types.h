#ifndef IZI_TYPES_H_
#define IZI_TYPES_H_

#include <stdint.h>
#include <stddef.h>

#include <util/atomic.h>

#include <izi/avr/config.h>
#include <izi/avr/utils/preprocessor.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Base AVR 8-bit type
typedef uint8_t IziBase_t;

//! Boolean type definition used by library
typedef uint8_t IziBool_t;

//! Definition for statements evaluating to false
#define IziFalse (0x00)

//! Definition for statements evaluating to true
#define IziTrue (!IziFalse)

//! Default size describing task
typedef uint16_t IziSize_t;

//! Type holding system time
typedef uint32_t IziTime_t;

//! Type for defining delays between events
typedef uint16_t IziDelay_t;

//! Type holding current tick number
#if IZI_SYSTEM_TICK_RATE > 256
typedef uint16_t IziTick_t;
#else
typedef uint8_t IziTick_t;
#endif

/*! \def IZI_MAX_DELAY
 * Definition of maximum available delay. Effectively
 * suspends task execution until event occurs. No earlier
 * wake up is possible.
 */
#define IZI_MAX_DELAY (0xFFFF)

//! Callback used for attaching user defined interrupt service routines.
typedef void (*IziIsrHandler_t)();

//! Creates block of code that will be executed atomically.
#define IZI_ATOMIC_BLOCK()  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)

//! Executes provided instruction in atomic block.
#define IZI_ATOMIC_INSTRUCTION(instruction) ATOMIC_BLOCK(ATOMIC_RESTORESTATE){ instruction; }

#ifndef _BV
#define _BV(x)                      (1 << (x))
#endif
#define _BV2(b1,b2)                 (_BV(b1)|_BV(b2))
#define _BV3(b1,b2,b3)              (_BV2(b1,b2)|_BV(b3))
#define _BV4(b1,b2,b3,b4)           (_BV3(b1,b2,b3)|_BV(b4))
#define _BV5(b1,b2,b3,b4,b5)        (_BV4(b1,b2,b3,b4)|_BV(b5))
#define _BV6(b1,b2,b3,b4,b5,b6)     (_BV5(b1,b2,b3,b4,b5)|_BV(b6))
#define _BV7(b1,b2,b3,b4,b5,b6,b7)  (_BV6(b1,b2,b3,b4,b5,b6)|_BV(b7))

#ifdef IZI_DEBUG
#define IZI_ASSERT(cond)            IZI_ATOMIC_INSTRUCTION(while((cond) == 0))
#else
#define IZI_ASSERT(cond)
#endif

#define IZI_GSTATIC_ASSERT(cond) enum { IZI_PP_CAT(STATIC_ASSERT_,__LINE__) = 1/(int)(cond) };

#define IZI_STATIC_ASSERT(cond) do { IZI_GSTATIC_ASSERT(cond) } while(0);

#ifdef __cplusplus
}
#endif

#endif /* IZI_TYPES_H_ */
