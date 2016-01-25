#ifndef IZI_TC1_H_
#define IZI_TC1_H_

#include <izi/avr/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	eIziTc1Int_ovf,
	eIziTc1Int_ocmpB,
	eIziTc1Int_ocmpA,
	eIziTc1Int_icpt,
	TC1_ISR_COUNT
} ETc1IntSource;

typedef enum {
	eIziTc1Clk_stop,
	eIziTc1Clk_fcpu,
	eIziTc1Clk_div8,
	eIziTc1Clk_div64,
	eIziTc1Clk_div256,
	eIziTc1Clk_div1024,
	eIziTc1Clk_extFall,
	eIziTc1Clk_extRaise
} ETc1ClockSource;

typedef enum {
	eIziTc1Wgm_normal,
	eIziTc1Wgm_pwmPhase8,
	eIziTc1Wgm_pwmPhase9,
	eIziTc1Wgm_pwmPhase10,
	eIziTc1Wgm_ctc_OCR1A,
	eIziTc1Wgm_pwmFast8,
	eIziTc1Wgm_pwmFast9,
	eIziTc1Wgm_pwmFast10,
	eIziTc1Wgm_pwmFreq_ICR1,
	eIziTc1Wgm_pwmFreq_OCR1A,
	eIziTc1Wgm_pwmPhase_ICR1,
	eIziTc1Wgm_pwmPhase_OCR1A,
	eIziTc1Wgm_ctc_ICR1,
	eIziTc1Wgm_reserved,
	eIziTc1Wgm_pwmFast_ICR1,
	eIziTc1Wgm_pwmFast_OCR1A
} ETc1WaveGenerationMode;

typedef enum {
	eIziTc1cm_normal,
	eIziTc1cm_toggle,
	eIziTc1cm_clr,
	eIziTc1cm_set
} ETc1OutCmpMode;

typedef uint16_t IziTc1Value_t;

void iziTc1Init(ETc1ClockSource cs, ETc1WaveGenerationMode wgm,
		ETc1OutCmpMode ocmA, ETc1OutCmpMode ocmB);

void iziTc1SetOutputCmpValueA(IziTc1Value_t value);

void iziTc1SetOutputCmpValueB(IziTc1Value_t value);

void iziTc1SetCounterValue(IziTc1Value_t value);

#if (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC1A) && (IZI_SYSTEM_TICK_SOURCE != IZI_SYSTEM_TICK_TC1B)

void iziTc1EnableInterrupt(ETc1IntSource intSource);

void iziTc1DisableInterrupt(ETc1IntSource intSource);

void iziTc1AttachHandler(ETc1IntSource intSource, IziIsrHandler_t handler);

void iziTc1DetachHandler(ETc1IntSource intSource);

#endif

#ifdef __cplusplus
}
#endif


#endif /* IZI_TC1_H_ */
