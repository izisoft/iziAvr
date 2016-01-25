#define DBGU_SGN 	"FPM"
#define DBGU_FLAG	FEAT_DBG_FPM

//#includes
#include <izi/avr/utils/fpm.h>

//#defines
#define IZI_FPM_BITS	16
#define IZI_FPM_LOP		((IZI_FPM_BITS+1)/2)
#define IZI_FPM_ROP		(IZI_FPM_BITS/2)
#define IZI_FPM_MASK	((1<<IZI_FPM_BITS)-1)

//#globals

//-------------------------------------------------------
//#function iziFpmAdd
TIziFpm iziFpmAdd(TIziFpm a, TIziFpm b) 
{
	return a+b;
}
//#function iziFpmAddi
TIziFpm iziFpmAddi(TIziFpm a, int32_t  b)
{
	return a + (b << IZI_FPM_BITS);
}
//#function iziFpmAddf
TIziFpm iziFpmAddf(TIziFpm a, double b) 
{
	TIziFpm c = iziFpmFromFloat(b);
	return a+c;
}
//-------------------------------------------------------
//#function iziFpmSub
TIziFpm iziFpmSub(TIziFpm a, TIziFpm b) 
{
	return a-b;
}
//#function iziFpmSub
TIziFpm iziFpmSubi(TIziFpm a, int32_t b) 
{
	return a-(b << IZI_FPM_BITS);
}
//#function iziFpmSub
TIziFpm iziFpmSubf(TIziFpm a, double b) 
{
	TIziFpm c = iziFpmFromFloat(b);
	return a-c;
}
//-------------------------------------------------------
//#function iziFpmMul
TIziFpm iziFpmMul(TIziFpm a, TIziFpm b) 
{
	return ((a >> IZI_FPM_LOP)*(b >> IZI_FPM_ROP));
}
//#function iziFpmMulf
TIziFpm iziFpmMulf(TIziFpm a, double b) 
{
	TIziFpm c = iziFpmFromFloat(b);
	return ((a >> IZI_FPM_LOP)*(c >> IZI_FPM_ROP));
}
//#function iziFpmMuli
TIziFpm iziFpmMuli(TIziFpm a, int32_t b) 
{
	return (a*b);
}
//-------------------------------------------------------
//#function iziFpmDiv
TIziFpm iziFpmDiv(TIziFpm a, TIziFpm b) 
{
	//To fix!!!
	return (a<<IZI_FPM_BITS)/b;
}
//#function iziFpmFromFloat
TIziFpm iziFpmFromFloat(double a)
{
	TIziFpm retVal;
	asm volatile(
		"LDI R26,150-%2		\n\t"
		"BST %D0,7			\n\t"
		"ADD %C0,%C0		\n\t"
		"ADC %D0,%D0		\n\t"	//Only exponent in D0
		"SUB R26,%D0		\n\t"
		"LSR %C0			\n\t"
		"ORI %C0,0x80 		\n\t"
		"CLR %D0			\n\t"	//Positive number is ready
		"AND R26,R26		\n\t"
		"BREQ L_izi3%=		\n\t"
		"BRGE L_izi2%=		\n\t"
		"NEG R26			\n\t"
		"L_izi1%=:"
		"ADD %A0,%A0		\n\t"	//Shift left
		"ADC %B0,%B0		\n\t"
		"ADC %C0,%C0		\n\t"
		"ADC %D0,%D0		\n\t"
		"DEC R26			\n\t"
		"BRNE L_izi1%=		\n\t"
		"RJMP L_izi3%=		\n\t"
		"L_izi2%=:"
		"ASR %D0			\n\t"	//Shift right
		"ROR %C0			\n\t"
		"ROR %B0			\n\t"
		"ROR %A0			\n\t"
		"DEC R26			\n\t"
		"BRNE L_izi2%=		\n\t"
		"L_izi3%=:"
		"BRTC L_izi4%=		\n\t"
		"COM %D0			\n\t" 	//Make negative
		"COM %C0			\n\t"
		"COM %B0			\n\t"
		"NEG %A0			\n\t"
		"L_izi4%=:"
		: "=r" (retVal)
		: "0" (a), "M" ((uint8_t)(IZI_FPM_BITS))
		: "r26"
	);
	return retVal;
}
//#function iziFpmFromInt
TIziFpm iziFpmFromInt(int32_t a) 
{
	return (a << IZI_FPM_BITS);
}
//#function iziFpmToFloat
double iziFpmToFloat(TIziFpm a)
{
	// Can be done in assempbly
	return (a / ((int32_t)1 << IZI_FPM_BITS));
}
//#function iziFpmToInt
int32_t iziFpmToInt(TIziFpm a)
{
	return (a >> IZI_FPM_BITS);
}
