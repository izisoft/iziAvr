#include <izi/avr/config.h>
#include <izi/avr/core/task.h>
#include <izi/avr/utils/preprocessor.h>

#include "iziDevicePriv.h"

#if IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC0
#include <izi/avr/driver/tc0.h>
#elif (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1A) || (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1B)
#include <izi/avr/driver/tc1.h>
#elif (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC2) || (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC2_ASYNC)
#include <izi/avr/driver/tc2.h>
#endif


#if IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC2_ASYNC
#define IZI_SYSTEM_FREQ IZI_CPU_FREQ
#else
#define IZI_SYSTEM_FREQ IZI_ASYNC_CLOCK_SPEED
#endif

#if (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1A) || (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1B)
#define IZI_SYSTEM_TC_SIZE 65536
#else
#define IZI_SYSTEM_TC_SIZE 256
#endif

#if (IZI_SYSTEM_TICK_RATE == 0) || (IZI_SYSTEM_TICK_RATE > 1000)
#error "Invalid value for IZI_SYSTEM_TICK_RATE"
#endif

#define IZI_CLK_PER_TICK (IZI_SYSTEM_FREQ / IZI_SYSTEM_TICK_RATE)

#if IZI_SYSTEM_TC_SIZE >= IZI_CLK_PER_TICK
#   define IZI_TC_TICK_PRESCALER 1
#   define IZI_TC_TICK_PRESCALER_SFX fcpu
#elif (IZI_SYSTEM_TC_SIZE * 8) >= IZI_CLK_PER_TICK
#   define IZI_TC_TICK_PRESCALER 8
#   define IZI_TC_TICK_PRESCALER_SFX div8
#elif (IZI_SYSTEM_TC_SIZE * 64) >= IZI_CLK_PER_TICK
#   define IZI_TC_TICK_PRESCALER 64
#   define IZI_TC_TICK_PRESCALER_SFX div64
#elif (IZI_SYSTEM_TC_SIZE * 256) >= IZI_CLK_PER_TICK
#   define IZI_TC_TICK_PRESCALER 256
#   define IZI_TC_TICK_PRESCALER_SFX div256
#elif (IZI_SYSTEM_TC_SIZE * 1024) >= IZI_CLK_PER_TICK
#   define IZI_TC_TICK_PRESCALER 1024
#   define IZI_TC_TICK_PRESCALER_SFX div1024
#else
#   error "Unable to achieve target tick rate"
#endif

#define IZI_SYSTEM_TICK_OCV ((2 * IZI_SYSTEM_FREQ / IZI_TC_TICK_PRESCALER / IZI_SYSTEM_TICK_RATE + 1) / 2 - 1)

void iziDeviceInitSystemClock()
{
#if IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC0
    iziTc0Init(IZI_PP_CAT(eIziTc0Clk_,IZI_TC_TICK_PRESCALER_SFX), eIziTc0Wgm_ctc);
    iziTc0SetOutputCmpValue(IZI_SYSTEM_TICK_OCV);
#elif (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1A)
    iziTc1Init(IZI_PP_CAT(eIziTc1Clk_,IZI_TC_TICK_PRESCALER_SFX), eIziTc1Wgm_ctc, eIziTc1cm_normal, eIziTc1cm_normal);
    iziTc1SetOutputCmpValueA(IZI_SYSTEM_TICK_OCV);
#elif (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC1B)
    iziTc1Init(IZI_PP_CAT(eIziTc1Clk_,IZI_TC_TICK_PRESCALER_SFX), eIziTc1Wgm_ctc, eIziTc1cm_normal, eIziTc1cm_normal);
    iziTc1SetOutputCmpValueB(IZI_SYSTEM_TICK_OCV);
#elif (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC2)
    iziTc2Init(IZI_PP_CAT(eIziTc2Clk_,IZI_TC_TICK_PRESCALER_SFX), eIziTc2Wgm_ctc, eIziTc2cm_normal, IziFalse);
    iziTc2SetOutputCmpValue(IZI_SYSTEM_TICK_OCV);
#elif (IZI_SYSTEM_TICK_SOURCE == IZI_SYSTEM_TICK_TC2_ASYNC)
    iziTc2Init(IZI_PP_CAT(eIziTc2Clk_,IZI_TC_TICK_PRESCALER_SFX), eIziTc2Wgm_ctc, eIziTc2cm_normal, IziTrue);
    iziTc2SetOutputCmpValue(IZI_SYSTEM_TICK_OCV);
#endif
}
