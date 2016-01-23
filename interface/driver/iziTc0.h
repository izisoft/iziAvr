#ifndef IZITC0_H_
#define IZITC0_H_

#include <iziAvr.h>
#include <iziConf.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	eIziTc0Int_ovf,
	eIziTc0Int_ocmp,
	TC0_ISR_COUNT
} ETc0IntSource;

typedef enum {
	eIziTc0Clk_stop,
	eIziTc0Clk_fcpu,
	eIziTc0Clk_div8,
	eIziTc0Clk_div64,
	eIziTc0Clk_div256,
	eIziTc0Clk_div1024,
	eIziTc0Clk_extFall,
	eIziTc0Clk_extRaise
} ETc0ClockSource;

typedef enum {
	eIziTc0Wgm_normal,
	eIziTc0Wgm_pwmPhase,
	eIziTc0Wgm_ctc,
	eIziTc0Wgm_pwmFast
} ETc0WaveGenerationMode;

typedef enum {
	eIziTcOcm_normal,
	eIziTcOcm_toggle,
	eIziTcOcm_clr,
	eIziTcOcm_set
} ETc0OutCmpMode;

typedef uint8_t IziTc0Value_t;

void iziTc0Init(ETc0ClockSource cs, ETc0WaveGenerationMode wgm, ETc0OutCmpMode ocm);

void iziTc0SetOutputCmpValue(IziTc0Value_t value);

void iziTc0SetCounterValue(IziTc0Value_t value);

#if IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC0

void iziTc0EnableInterrupt(ETc0IntSource intSource);

void iziTc0DisableInterrupt(ETc0IntSource intSource);

void iziTc0AttachHandler(ETc0IntSource intSource, IziIsrHandler_t handler);

void iziTc0DetachHandler(ETc0IntSource intSource);

#endif

#ifdef __cplusplus
}
#endif

#endif /* IZITC0_H_ */
