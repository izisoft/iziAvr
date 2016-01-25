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

void iziDeviceInitSystemClock();

void iziKernelIdleHook();

void iziKernelStartupHook();

int iziMain();

typedef uint8_t IziBase_t;

typedef uint8_t IziBool_t;
#define IziFalse (0x00)
#define IziTrue (!IziFalse)

typedef uint16_t IziSize_t;

typedef uint32_t IziTime_t;
typedef uint16_t IziDelay_t;

#if IZI_SYSTEM_TICK_RATE > 256
typedef uint16_t IziTick_t;
#else
typedef uint8_t IziTick_t;
#endif

#define IZI_MAX_DELAY (0xFFFF)

/** \typedef IziRegister_t
	Register type used in iziavr library.
*/
typedef volatile uint8_t IziRegister_t;

typedef void (*IziIsrHandler_t)();

#define IZI_ATOMIC_BLOCK()  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)

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
