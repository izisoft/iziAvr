#include <utils/iziCrc.h>
#include <util/crc16.h>

IziCrc_t iziCrcCalculate(const char* data, size_t dataSize)
{
	IziCrc_t calculatedCrc = 0;
	while(dataSize--) {
#ifdef IZI_CRC_16
			calculatedCrc = iziCrc16(calculatedCrc,*(data++));
#else
			calculatedCrc = iziCrc8(calculatedCrc,*(data++));
#endif
	}
	return calculatedCrc;
}

uint8_t iziCrc8(uint8_t crc,uint8_t data)
{
	uint8_t __i, __pattern;
	
#if CC != avr-gcc
	/* Flexible C code */
	crc = crc ^ data;
	for (__i = 0; __i < 8; __i++)
	{
		if(crc & 0x01)
		{
			crc = (crc >> 1) ^ 0x8C;
		}
		else
		{
			crc >>= 1;
		}
	}
#else
	/* AVR optimized assembler code */
	__asm__ __volatile__ (
		"	eor	%0, %4                      \n\t"
		"	ldi	%1, 8                       \n\t"
		"	ldi	%2, 0x8C                    \n\t"
		"1:	bst	%0, 0                       \n\t"
		"	lsr	%0                          \n\t"
		"	brtc	2f                      \n\t"
		"	eor	%0, %2                      \n\t"
		"2:	dec	%1                          \n\t"
		"	brne	1b                      \n\t"
		: "=r" (crc), "=d" (__i), "=d" (__pattern)
		: "0" (crc), "r" (data));
#endif
	return crc;
}

uint16_t iziCrc16(uint16_t* crc, uint8_t data) 
{
	return 0;
}

