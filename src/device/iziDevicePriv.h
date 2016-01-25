#ifndef IZI_DEVICEPRIV_H_
#define IZI_DEVICEPRIV_H_

#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <izi/avr/core.h>
#include <izi/avr/utils/preprocessor.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void iziDeviceInitSystemClock();
extern void iziDeviceInitTaskStack(TIziTask *task, void (*taskFunction)(void));

#if defined(atmega8)

#define IZI_DEVIE_SYMBOL M8
#define IZI_DEVICE_RESOURCES (8192,1024,512)

#elif defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || defined(__AVR_ATmega161__) || defined(__AVR_ATmega162__)

#define IZI_DEVICE_SYMBOL M16
#define IZI_DEVICE_RESOURCES (16384,1024,512)

#elif defined(__AVR_ATmega32__) || defined(__AVR_ATmega323__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324A__)

#define IZI_DEVICE_SYMBOL M32
#define IZI_DEVICE_RESOURCES (32768,2048,1024)

#else

#error "Unsupported device type"

#endif

#define IZI_SAVE_CONTEXT()                                     \
asm volatile ( "push  r0                                \n\t"  \
               "in    r0, __SREG__                      \n\t"  \
               "cli                                     \n\t"  \
               "push  r0                                \n\t"  \
               "push  r1                                \n\t"  \
               "eor   r1,r1                             \n\t"  \
               "push  r2                                \n\t"  \
               "push  r3                                \n\t"  \
               "push  r4                                \n\t"  \
               "push  r5                                \n\t"  \
               "push  r6                                \n\t"  \
               "push  r7                                \n\t"  \
               "push  r8                                \n\t"  \
               "push  r9                                \n\t"  \
               "push  r10                               \n\t"  \
               "push  r11                               \n\t"  \
               "push  r12                               \n\t"  \
               "push  r13                               \n\t"  \
               "push  r14                               \n\t"  \
               "push  r15                               \n\t"  \
               "push  r16                               \n\t"  \
               "push  r17                               \n\t"  \
               "push  r18                               \n\t"  \
               "push  r19                               \n\t"  \
               "push  r20                               \n\t"  \
               "push  r21                               \n\t"  \
               "push  r22                               \n\t"  \
               "push  r23                               \n\t"  \
               "push  r24                               \n\t"  \
               "push  r25                               \n\t"  \
               "push  r26                               \n\t"  \
               "push  r27                               \n\t"  \
               "push  r28                               \n\t"  \
               "push  r29                               \n\t"  \
               "push  r30                               \n\t"  \
               "push  r31                               \n\t"  \
               "lds   r26, gIziCurrentTask              \n\t"  \
               "lds   r27, gIziCurrentTask + 1          \n\t"  \
               "in    r0, __SP_L__                      \n\t"  \
               "st    x+, r0                            \n\t"  \
               "in    r0, __SP_H__                      \n\t"  \
               "st    x+, r0                            \n\t"  \
              )

#define IZI_RESTORE_CONTEXT()                                  \
asm volatile ( "lds   r26, gIziCurrentTask              \n\t"  \
               "lds   r27, gIziCurrentTask + 1          \n\t"  \
               "ld    r28, x+                           \n\t"  \
               "out   __SP_L__, r28                     \n\t"  \
               "ld    r29, x+                           \n\t"  \
               "out   __SP_H__, r29                     \n\t"  \
               "pop   r31                               \n\t"  \
               "pop   r30                               \n\t"  \
               "pop   r29                               \n\t"  \
               "pop   r28                               \n\t"  \
               "pop   r27                               \n\t"  \
               "pop   r26                               \n\t"  \
               "pop   r25                               \n\t"  \
               "pop   r24                               \n\t"  \
               "pop   r23                               \n\t"  \
               "pop   r22                               \n\t"  \
               "pop   r21                               \n\t"  \
               "pop   r20                               \n\t"  \
               "pop   r19                               \n\t"  \
               "pop   r18                               \n\t"  \
               "pop   r17                               \n\t"  \
               "pop   r16                               \n\t"  \
               "pop   r15                               \n\t"  \
               "pop   r14                               \n\t"  \
               "pop   r13                               \n\t"  \
               "pop   r12                               \n\t"  \
               "pop   r11                               \n\t"  \
               "pop   r10                               \n\t"  \
               "pop   r9                                \n\t"  \
               "pop   r8                                \n\t"  \
               "pop   r7                                \n\t"  \
               "pop   r6                                \n\t"  \
               "pop   r5                                \n\t"  \
               "pop   r4                                \n\t"  \
               "pop   r3                                \n\t"  \
               "pop   r2                                \n\t"  \
               "pop   r1                                \n\t"  \
               "pop   r0                                \n\t"  \
               "out   __SREG__, r0                      \n\t"  \
               "pop   r0                                \n\t"  \
              )

#define IZI_RETURN()                    asm volatile ( "ret" )

#define IZI_RETURN_INTERRUPT()          asm volatile ( "reti" )

#define IZI_DISABLE_INTERRUPTS()        asm volatile ( "cli" )

#define IZI_ENABLEE_INTERRUPTS()        asm volatile ( "sei" )

#define IZI_SET_KERNEL_STACK()          SP = RAMEND

#if IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC0
#	define IZI_SYSTEM_INTERRUPT() ISR(TIMER0_COMP_vect,ISR_NAKED)
#elif IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1A
#	define IZI_SYSTEM_INTERRUPT() ISR(TIMER1_COMPA_vect,ISR_NAKED)
#elif IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1B
#	define IZI_SYSTEM_INTERRUPT() ISR(TIMER1_COMPB_vect,ISR_NAKED)
#elif IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC2
#	define IZI_SYSTEM_INTERRUPT() ISR(TIMER2_COMP_vect,ISR_NAKED)
#elif IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC2_ASYNC
#	define IZI_SYSTEM_INTERRUPT() ISR(TIMER2_COMP_vect,ISR_NAKED)
#endif

#define IZI_IS_SYSTEM_INTERRUPT(isr)    IZI_IS_SAME_ISR(IZI_SYSTEM_TICK_SIGNAL, isr)

#define IZI_ENABLE_SYSTEM_INTERRUPT()   TIMSK |= _BV(OCIE2);

#define IZI_INIT_MEM_MANAGER()          __malloc_heap_end = (char*)(RAMEND-48); \
                                        __malloc_margin = 0

#define IZI_SREG_INIT_VALUE             0x80

#define IZI_LOOP_CRITICAL()             cli(); for(;;)

#define IZI_RAM_BEGIN_ADDR              (0x0060)

#define IZI_CONST(type,name)            type __attribute__((__progmem__)) name
#define IZI_CONST_READ_BYTE(addr)       pgm_read_byte(addr)
#define IZI_CONST_READ_WORD(addr)       pgm_read_word(addr)
#define IZI_CONST_READ_DWORD(addr)      pgm_read_dword(addr)
#define IZI_CONST_READ_FLOAT(addr)      pgm_read_float(addr)

#define IZI_PROGMEM                     __attribute__((__progmem__))
#define IZI_PSTR(s)                     (__extension__({static char __c[] IZI_PROGMEM = (s); &__c[0];}))

#define IZI_DEVICE_FLASH_SIZE      IZI_PP_TUPLE_GET(IZI_DEVICE_RESOURCES, 0)
#define IZI_DEVICE_RAM_SIZE        IZI_PP_TUPLE_GET(IZI_DEVICE_RESOURCES, 1)
#define IZI_DEVICE_EEPROM_SIZE     IZI_PP_TUPLE_GET(IZI_DEVICE_RESOURCES, 2)

// Eeprom map
#define IZI_EE_KERNEL_RESERVED         (20)
#define IZI_EE_USER_RESERVED           (100)
#define IZI_EE_STORAGE_RESERVED        (IZI_DEVICE_EEPROM_SIZE - IZI_EE_KERNEL_RESERVED - IZI_EE_USER_RESERVED)
#define IZI_EE_STORAGE_START           (IZI_EE_KERNEL_RESERVED)
#define IZI_EE_USER_START              (IZI_DEVICE_EEPROM_SIZE - IZI_EE_USER_RESERVED)

#define IZI_ASYNC_CLOCK_SPEED (32768)

#ifdef __cplusplus
}
#endif

#endif /* IZI_DEVICEPRIV_H_ */
