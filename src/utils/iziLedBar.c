#include <izi/avr/utils/ledBar.h>

//void iziLedBarSet(TIziLedBar *ledBar,uint16_t level)
//{
//	uint8_t i;
//	uint8_t j = ledBar->maxDisplayed;
//	for(i = ledBar->ledCount; i > 0; i--)
//	{
//		if((level > *(ledBar->triggerLevels + i-1))&&(j > 0))
//		{
//			iziPioClear(ledBar->leds + i-1);
//			j--;
//		}
//		else
//		{
//			iziPioSet(ledBar->leds + i-1);
//		}
//	}
//}
