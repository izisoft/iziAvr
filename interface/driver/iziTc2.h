#ifndef IZITC2_H_
#define IZITC2_H_

#include <iziAvr.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	eIziTc2Int_ovf,
	eIziTc2Int_ocmp,
	TC2_ISR_COUNT
} ETc2IntSource;

typedef enum {
	eIziTc2Clk_stop,
	eIziTc2Clk_fcpu,
	eIziTc2Clk_div8,
	eIziTc2Clk_div64,
	eIziTc2Clk_div256,
	eIziTc2Clk_div1024,
	eIziTc2Clk_extFall,
	eIziTc2Clk_extRaise,
	eIziTc2Clk_reserved
} ETc2ClockSource;

typedef enum {
	eIziTc2Wgm_normal,
	eIziTc2Wgm_pwmPhase,
	eIziTc2Wgm_ctc,
	eIziTc2Wgm_pwmFast
} ETc2WaveGenerationMode;

typedef enum {
	eIziTc2cm_normal,
	eIziTc2cm_toggle,
	eIziTc2cm_clr,
	eIziTc2cm_set
} ETc2OutCmpMode;

typedef uint8_t IziTc2Value_t;

void iziTc2Init(ETc2ClockSource cs, ETc2WaveGenerationMode wgm, ETc2OutCmpMode ocm, IziBool_t async);

void iziTc2SetOutputCmpValue(IziTc2Value_t value);

void iziTc2SetCounterValue(IziTc2Value_t value);

#if (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC2) && (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC2_ASYNC)

void iziTc2EnableInterrupt(ETc2IntSource intSource);

void iziTc2DisableInterrupt(ETc2IntSource intSource);

void iziTc2AttachHandler(ETc2IntSource intSource, IziIsrHandler_t handler);

void iziTc2DetachHandler(ETc2IntSource intSource);

#endif

#ifdef __cplusplus
}
#endif

#endif /* IZITC2_H_ */
