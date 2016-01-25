#include <avr/io.h>
#include <avr/interrupt.h>

#include <izi/avr/core/semaphore.h>
#include <izi/avr/core/mutex.h>
#include <izi/avr/core/kernel.h>
#include <izi/avr/driver/eeprom.h>

#include <device/iziDevicePriv.h>

TIziSemaphore iziEepromSemph;
TIziMutex	iziEepromMutex;

#define ENABLE_EEPROM_INT()		EECR |= _BV(EERIE)
#define DISABLE_EEPROM_INT()	EECR &= ~_BV(EERIE)

#define ENABLE_EEPROM_WRITE()									\
	asm volatile (	"sbi	%[__eecr], %[__eemwe]	\n\t"		\
					"sbi	%[__eecr], %[__eewe]	\n\t"		\
					"sbi	%[__eecr], %[__eerie]	\n\t"		\
					:: 											\
					[__eecr]  "i" (_SFR_IO_ADDR(EECR)),			\
					[__eemwe] "i" (EEMWE),						\
					[__eewe]  "i" (EEWE),						\
					[__eerie] "i" (EERIE)						\
	)

#define ENABLE_EEPROM_READ()									\
	asm volatile (	"sbi	%[__eecr], %[__eere]	\n\t"		\
					:: 											\
					[__eecr] "i" (_SFR_IO_ADDR(EECR)),			\
					[__eere] "i" (EERE)							\
	)

//=====================================================================
void iziEepromInit( void )
{
	if(!iziKernelCheckState(eIziEepromInit))
	{
		iziSemaphoreCreate(&iziEepromSemph,IziFalse);
		iziMutexCreate(&iziEepromMutex);
		iziKernelSetState(eIziEepromInit);
	}
}

//=====================================================================
uint8_t iziEepromWriteNative(IziEepromAddr_t position, const char *data, uint8_t dataCount)
{
	if(iziMutexTake(&iziEepromMutex,IZI_MAX_DELAY) == IziTrue)
	{
		ENABLE_EEPROM_INT();
		iziSemaphoreTake(&iziEepromSemph,IZI_MAX_DELAY);
		while(dataCount--)
		{
			EEAR = position++;
			EEDR = *(data++);
			IZI_ATOMIC_INSTRUCTION(ENABLE_EEPROM_WRITE());
			iziSemaphoreTake(&iziEepromSemph,IZI_MAX_DELAY);
		}
		iziMutexGive(&iziEepromMutex);
		return dataCount;
	}
	return 0;
}
//--
uint8_t iziEepromReadNative(IziEepromAddr_t position, char *data, uint8_t dataCount)
{
	if(iziMutexTake(&iziEepromMutex,IZI_MAX_DELAY) == IziTrue)
	{
		ENABLE_EEPROM_INT();
		iziSemaphoreTake(&iziEepromSemph,IZI_MAX_DELAY);
		while(dataCount--)
		{
			EEAR = position++;
			ENABLE_EEPROM_READ();
			*(data++) = EEDR;
		}
		iziMutexGive(&iziEepromMutex);
		return dataCount;
	}
	return 0;
}

//=====================================================================
uint8_t iziEepromWrite(IziEepromAddr_t position, const char *data, uint8_t dataCount)
{
	if(position + dataCount + IZI_EE_USER_START > IZI_DEVICE_EEPROM_SIZE)
	{
		return 0;
	}
	return iziEepromWriteNative(position+IZI_EE_USER_START,data,dataCount);
}
//--
uint8_t iziEepromRead(IziEepromAddr_t position, char *data, uint8_t dataCount)
{
	if(position + dataCount + IZI_EE_USER_START > IZI_DEVICE_EEPROM_SIZE)
	{
		return 0;
	}
	return iziEepromReadNative(position+IZI_EE_USER_START,data,dataCount);
}

//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
ISR(EE_RDY_vect)
{
	IziBool_t needYeld;

	iziSemaphoreGiveFromIsr(&iziEepromSemph,&needYeld);
	DISABLE_EEPROM_INT();

	if(needYeld == IziTrue)
	{
		iziKernelYeld();
	}
}
