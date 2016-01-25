#ifndef IZI_EXTINT_H_
#define IZI_EXTINT_H_

#ifdef __cplusplus
extern "C" {
#endif

enum EIziExtIntSource {
	eIziExtInt2,
	eIziExtInt0,
	eIziExtInt1,
	IZI_EXT_INT_COUNT
};
typedef enum EIziExtIntSource TIziExtIntSource;

enum EIziExtInt0Sense {
	eIziExtInt0SenseLow,
	eIziExtInt0SenseChange,
	eIziExtInt0SenseFall,
	eIziExtInt0SenseRise
};
typedef enum EIziExtInt0Sense TIziExtInt0Sense;

enum EIziExtInt1Sense {
	eIziExtInt1SenseLow,
	eIziExtInt1SenseChange,
	eIziExtInt1SenseFall,
	eIziExtInt1SenseRise
};
typedef enum EIziExtInt1Sense TIziExtInt1Sense;

enum EIziExtInt2Sense {
	eIziExtInt2SenseFall,
	eIziExtInt2SenseRise
};
typedef enum EIziExtInt2Sense TIziExtInt2Sense;

void iziExtInt0SetSense(TIziExtInt0Sense sense);
void iziExtInt1SetSense(TIziExtInt1Sense sense);
void iziExtInt2SetSense(TIziExtInt2Sense sense);

void iziExtIntEnableInterrupt(TIziExtIntSource intSource);
void iziExtIntDisableInterrupt(TIziExtIntSource intSource);

void iziExtIntAttachHandler(TIziExtIntSource intSource, void (*handler)(void));
void iziExtIntDetachHandler(TIziExtIntSource intSource);

#ifdef __cplusplus
}
#endif

#endif /* IZI_EXTINT_H_ */
