#include <avr/interrupt.h>
#include <avr/io.h>

#include <iziConf.h>
#include <core/iziKernel.h>
#include <core/iziMutex.h>
#include <core/iziSemaphore.h>
#include <driver/iziTwi.h>
#include <device/iziDevicePriv.h>

// Master
#define IZI_TWI_START					0x08
#define IZI_TWI_REP_START				0x10
// Master Transmitter
#define IZI_TWI_MT_SLA_ACK				0x18
#define IZI_TWI_MT_SLA_NACK				0x20
#define IZI_TWI_MT_DATA_ACK				0x28
#define IZI_TWI_MT_DATA_NACK			0x30
// Master Receiver
#define IZI_TWI_MR_SLA_ACK				0x40
#define IZI_TWI_MR_SLA_NACK				0x48
#define IZI_TWI_MR_DATA_ACK				0x50
#define IZI_TWI_MR_DATA_NACK			0x58
// Common values
#define IZI_TWI_ARB_LOST				0x38
#define IZI_TWI_NO_INFO					0xF8
#define IZI_TWI_BUS_ERROR				0x00
#define TWI_STATUS()					(TWSR & 0xF8)


struct SIziTwi
{
	TIziTwiState state;

	uint8_t sla;

	uint8_t *sndBuff;
	uint8_t sndSize;

	uint8_t *recBuff;
	uint8_t recSize;
};

typedef volatile struct SIziTwi TIziTwi;

TIziTwi iziTwi;
TIziMutex iziTwiMutex;
TIziSemaphore iziTwiReady;

void iziTwiInit(uint8_t bitrateKHz)
{
	uint8_t presc = 0;
	uint16_t tmpTwbr = ((IZI_CPU_FREQ) / (8000UL * bitrateKHz)) - 2;
	while(tmpTwbr > 255)
	{
		tmpTwbr = tmpTwbr/4;
		if(presc++ > 3)
		{
			presc = 3;
			break;
		}
	}
	TWBR = tmpTwbr;
	TWSR &= ~(0x03);
	TWSR |= presc;

	TWCR = _BV2(TWEA,TWEN);

	if(!iziKernelCheckState(eIziTwiInit))
	{
		iziMutexCreate(&iziTwiMutex);
		iziSemaphoreCreate(&iziTwiReady,0);
		iziKernelSetState(eIziTwiInit);
	}
}

static void iziTwiSendStart()
{
	TWCR = _BV4(TWINT,TWSTA,TWEN,TWIE);
}

static void iziTwiSendStop()
{
	TWCR = _BV3(TWINT,TWSTO,TWEN);
}

static void iziTwiSendSla()
{
	TWDR = iziTwi.sla;
	TWCR = _BV3(TWINT,TWEN,TWIE);
}

static void iziTwiSendByte(uint8_t data)
{
	TWDR = data;
	TWCR = _BV3(TWINT,TWEN,TWIE);
}

static void iziTwiSendAck()
{
	TWCR = _BV4(TWINT,TWEA,TWEN,TWIE);
}

static void iziTwiSendNack()
{
	TWCR = _BV3(TWINT,TWEN,TWIE);
}

TIziTwiState iziTwiMasterSend(uint8_t addr, uint8_t *data, uint8_t dataCount)
{
	TIziTwiState retVal;

	iziMutexTake(&iziTwiMutex,IZI_MAX_DELAY);

	iziTwi.sla = (addr << 1);
	iziTwi.sndBuff = data;
	iziTwi.sndSize = dataCount;
	iziTwi.recSize = 0;
	iziTwi.state = eIziTwiBusy;

	iziTwiSendStart();
	iziSemaphoreTake(&iziTwiReady,IZI_MAX_DELAY);

	retVal = iziTwi.state;
	iziTwi.state = eIziTwiIdle;

	iziMutexGive(&iziTwiMutex);

	return retVal;
}
//===========================================================
TIziTwiState iziTwiMasterReceive(uint8_t addr, uint8_t *data, uint8_t dataCount)
{
	TIziTwiState retVal;

	iziMutexTake(&iziTwiMutex,IZI_MAX_DELAY);

	iziTwi.sla = (addr << 1) | 0x01;
	iziTwi.recBuff = data;
	iziTwi.recSize = dataCount;
	iziTwi.state = eIziTwiBusy;

	iziTwiSendStart();
	iziSemaphoreTake(&iziTwiReady,IZI_MAX_DELAY);

	retVal = iziTwi.state;
	iziTwi.state = eIziTwiIdle;

	iziMutexGive(&iziTwiMutex);

	return retVal;
}

//===========================================================
TIziTwiState iziTwiMasterSendReceive(uint8_t addr,
		uint8_t *sendBuffer, uint8_t sendSize,
		uint8_t *recBuffer, uint8_t recSize)
{
	TIziTwiState retVal;

	iziMutexTake(&iziTwiMutex,IZI_MAX_DELAY);

	iziTwi.sla = (addr << 1);
	iziTwi.sndBuff = sendBuffer;
	iziTwi.sndSize = sendSize;
	iziTwi.recBuff = recBuffer;
	iziTwi.recSize = recSize;
	iziTwi.state = eIziTwiBusy;

	iziTwiSendStart();
	iziSemaphoreTake(&iziTwiReady,IZI_MAX_DELAY);

	retVal = iziTwi.state;
	iziTwi.state = eIziTwiIdle;

	iziMutexGive(&iziTwiMutex);

	return retVal;
}
//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
// ISR for TWI in master mode
ISR(TWI_vect)
{
	switch(TWI_STATUS())
	{
	//=======================================================
	// Transmission start
	case IZI_TWI_START:
	case IZI_TWI_REP_START:
		iziTwiSendSla();
		break;

	//=======================================================
	// Master Transmitter Mode
	case IZI_TWI_MT_SLA_ACK:
	case IZI_TWI_MT_DATA_ACK:
		if((iziTwi.sndSize)-- > 0)
		{
			iziTwiSendByte(*((iziTwi.sndBuff)++));
		}
		else
		{
			iziTwi.state = eIziTwiMT_Ok;
		}
		break;
	//------------------------------------------
	case IZI_TWI_MT_SLA_NACK:
	case IZI_TWI_MT_DATA_NACK:
		iziTwi.state = eIziTwiMT_Error;
		break;

	//=======================================================
	// Master Receiver Mode
	case IZI_TWI_MR_SLA_ACK:
		if(iziTwi.recSize < 2)
		{
			iziTwiSendNack();
		}
		else
		{
			iziTwiSendAck();
		}
		break;
	//------------------------------------------
	case IZI_TWI_MR_SLA_NACK:
		iziTwi.state = eIziTwiMR_Error;
		break;
	//------------------------------------------
	case IZI_TWI_MR_DATA_ACK:
		*((iziTwi.recBuff)++) = TWDR;
		if((iziTwi.recSize)-- < 2)
		{
			iziTwiSendNack();
		}
		else
		{
			iziTwiSendAck();
		}
		break;
	//------------------------------------------
	case IZI_TWI_MR_DATA_NACK:
		*((iziTwi.recBuff)++) = TWDR;
		iziTwi.state = eIziTwiMR_Ok;
		break;

	//=======================================================
	// Default behavior
	default:
		iziTwi.state = eIziTwiError;
		break;
	}

	//=======================================================
	// State check
	if(iziTwi.state != eIziTwiBusy)
	{
		if((iziTwi.state == eIziTwiMT_Ok) && (iziTwi.recSize > 0))
		{
			iziTwi.sla = iziTwi.sla | 0x01;
			iziTwi.state = eIziTwiBusy;
			iziTwiSendStart();
		}
		else
		{
			iziTwiSendStop();
			IziBool_t needYeld;
			iziSemaphoreGiveFromIsr(&iziTwiReady,&needYeld);
			if(needYeld == IziTrue)
			{
				iziKernelYeld();
			}
		}
	}
}

