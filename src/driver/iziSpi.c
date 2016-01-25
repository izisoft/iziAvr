#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <izi/avr/config.h>
#include <izi/avr/driver/spi.h>
#include <izi/avr/core/memory.h>
#include <izi/avr/core/mutex.h>
#include <izi/avr/core/semaphore.h>

#include <core/kernel/iziKernelPriv.h>
#include <device/iziDevicePriv.h>

#define SS_PIN					eIziPinB4
#define MOSI_PIN				eIziPinB5
#define MISO_PIN				eIziPinB6
#define SCK_PIN					eIziPinB7

#define SPI_WAIT()				while(!(SPSR & (1<<SPIF)))

#define ENABLE_SPI_INT()		SPCR |= _BV(SPIE)
#define DISABLE_SPI_INT()		SPCR &= ~_BV(SPIE)

IZI_CONST(uint8_t,SpiClock[]) = {4,0,5,1,6,2,3};
	
TIziSpiUnit* pCurrentSpiUnit = NULL;

TIziMutex iziSpiMutex;
TIziSemaphore iziSpiSemph;

char *iziSpiData;
uint8_t iziSpiDataCount;

//=====================================================================
static void iziSpiInit()
{	
	if(!IZI_GET_STATE(eIziSpiInit))
	{
		iziPinSetType(SS_PIN, eIziPinOut);
		iziPinSetType(MOSI_PIN, eIziPinOut);
		iziPinSetType(MISO_PIN, eIziPinIn);
		iziPinSetType(SCK_PIN, eIziPinOut);

		iziPinSet(SS_PIN, IziTrue);
	
		iziMutexCreate(&iziSpiMutex);
		iziSemaphoreCreate(&iziSpiSemph,IziFalse);
	}
}

//=====================================================================
TIziSpiUnit* iziSpiAddUnit(EIziPin csPin,
						TIziSpiTrasmissionMode spiMode,
						uint16_t wantdeClock_kHz)
{
	TIziSpiUnit* pSpiUnit = NULL;
	uint8_t xSPCR;
	uint8_t xSPSR;
	uint8_t xDivider=2;
	uint8_t xDividerIndex = 0;
	
	iziSpiInit();

	iziPinSetType(csPin, eIziPinOut);

	while(((uint16_t)(IZI_CPU_FREQ/1000)/xDivider > wantdeClock_kHz) && (xDivider < 128))
	{
		xDivider *= 2;
		xDividerIndex++;
	}
	
	xSPCR = (eIziSpiMasterMode << MSTR) | (eIziSpiMsb << DORD) | (1 << SPE);
	xSPCR |= (spiMode << CPHA);
	xSPCR |= (IZI_CONST_READ_BYTE(SpiClock+xDividerIndex) & 0x03);
	
	if(IZI_CONST_READ_BYTE(SpiClock+xDividerIndex) > 3)
	{
		xSPSR = (1 << SPI2X);
	}
	
	pSpiUnit =  (TIziSpiUnit*)iziMalloc(sizeof(TIziSpiUnit));
	
	pSpiUnit->pCsPin = csPin;
	pSpiUnit->xSPCR = xSPCR;
	pSpiUnit->xSPSR = xSPSR;
	
	iziSpiDeselectUnit(pSpiUnit);

	return pSpiUnit;
}

//=====================================================================
// Depreciated function, will be removed
uint8_t iziSpiSendByte(uint8_t data, TIziSpiUnit* pSpiUnit)
{
	if(iziMutexTake(&iziSpiMutex,IZI_MAX_DELAY) == IziTrue)
	{
		if(pCurrentSpiUnit != pSpiUnit)
		{
			SPCR = pSpiUnit->xSPCR;
			SPSR = pSpiUnit->xSPSR;
			pCurrentSpiUnit = pSpiUnit;
		}

		iziPinSet(pSpiUnit->pCsPin, IziFalse);

		SPDR = data;
		SPI_WAIT();

		iziPinSet(pSpiUnit->pCsPin, IziTrue);

		iziMutexGive(&iziSpiMutex);
		return data;
	}
	else
	{
		return 0x00;
	}
}

//=====================================================================
// Depreciated function, will be removed
uint16_t iziSpiSendWord(uint16_t data,TIziSpiUnit* pSpiUnit)
{
	uint16_t retValue;
	if(iziMutexTake(&iziSpiMutex,IZI_MAX_DELAY) == IziTrue)
	{
		if(pCurrentSpiUnit != pSpiUnit)
		{
			SPCR = pSpiUnit->xSPCR;
			SPSR = pSpiUnit->xSPSR;
			pCurrentSpiUnit = pSpiUnit;
		}

		iziPinSet(pSpiUnit->pCsPin, IziFalse);

		SPDR = (data >> 8) & 0xFF;
		SPI_WAIT();
		retValue = (SPDR << 8) & 0xFF00;

		SPDR = data & 0xFF;
		SPI_WAIT();
		retValue |= (SPDR & 0x00FF);

		iziPinSet(pSpiUnit->pCsPin, IziTrue);

		iziMutexGive(&iziSpiMutex);
		return retValue;
	}
	else
	{
		return 0x0000;
	}
}

//=====================================================================
void iziSpiSendStream(TIziSpiUnit* spiUnit, char* data, uint8_t dataCount)
{
	if(pCurrentSpiUnit != spiUnit)
	{
		SPCR = spiUnit->xSPCR;
		SPSR = spiUnit->xSPSR;
		pCurrentSpiUnit = spiUnit;
	}

	iziSpiData = data;
	iziSpiDataCount = dataCount;

	SPDR = *iziSpiData;
	ENABLE_SPI_INT();
	iziSemaphoreTake(&iziSpiSemph,IZI_MAX_DELAY);
}

//=====================================================================
void iziSpiSelectUnit(TIziSpiUnit* spiUnit)
{
	if(iziMutexTake(&iziSpiMutex,IZI_MAX_DELAY) == IziTrue)
	{
		iziPinSet(spiUnit->pCsPin, IziFalse);
	}
}
//--
void iziSpiDeselectUnit(TIziSpiUnit* spiUnit)
{
	iziPinSet(spiUnit->pCsPin, IziTrue);
	iziMutexGive(&iziSpiMutex);
}

//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
ISR(SPI_STC_vect)
{
	IziBool_t needYeld;

	*(iziSpiData) = SPDR;

	if(--iziSpiDataCount > 0)
	{
		SPDR = *(++iziSpiData);
	}
	else
	{
		iziSemaphoreGiveFromIsr(&iziSpiSemph,&needYeld);
		DISABLE_SPI_INT();
		if(needYeld == IziTrue)
		{
			iziKernelYeld();
		}
	}
}
