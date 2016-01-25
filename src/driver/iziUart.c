#include <stdarg.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include <izi/avr/core/queue.h>
#include <izi/avr/driver/uart.h>

#include <core/kernel/iziKernelPriv.h>
#include <device/iziDevicePriv.h>

#define ENABLE_UART_TXINT()				UCSRB |= _BV(UDRIE)
#define DISABLE_UART_TXINT()			UCSRB &= ~_BV(UDRIE)

#define ENABLE_UART_RXINT()				UCSRB |= _BV(RXCIE)
#define DISABLE_UART_RXINT()			UCSRB &= ~_BV(RXCIE)

#define ENABLE_TX_COMPLETE_INT()		UCSRB |= _BV(TXCIE)
#define DISABLE_TX_COMPLETE_INT()		UCSRB &= ~_BV(TXCIE)

#define IZI_UART_TX_QUEUE_SIZE			8
#define IZI_UART_RX_QUEUE_SIZE			8

volatile char *iziUartDataPtr;
volatile uint8_t iziUartDataCount;

volatile TIziQueue iziUartTxQueue;
volatile TIziQueue iziUartRxQueue;

//=====================================================================
static void iziUartInit( void )
{
	if(!IZI_GET_STATE(eIziUartInit))
	{
		iziQueueCreate(&iziUartTxQueue,IZI_UART_TX_QUEUE_SIZE,1);
		iziQueueCreate(&iziUartRxQueue,IZI_UART_RX_QUEUE_SIZE,1);

		DISABLE_UART_TXINT();
		DISABLE_UART_RXINT();
		UCSRB |= (1<<RXEN)|(1<<TXEN);
		UCSRC |= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);

		IZI_SET_STATE(eIziUartInit);
	}
}

//=====================================================================
void iziUartConfigureUnit(TIziUartInterface interface,uint32_t baudrate)
{
	uint32_t baudRateCounter;
	iziUartInit();

	IZI_ATOMIC_BLOCK()
	{
		baudRateCounter = (IZI_CPU_FREQ/(16*baudrate))-1;
		UBRRL = (baudRateCounter & 0xFF);
		baudRateCounter >>= 8;
		UBRRH = (baudRateCounter & 0xFF);
	}
}

//=====================================================================
void iziUartPutchar(TIziUartInterface interface, char c)
{
	iziQueuePutFromTask(&iziUartTxQueue,(const void*)(&c),IZI_MAX_DELAY);
	ENABLE_UART_TXINT();
}
//--
char iziUartGetchar(TIziUartInterface interface)
{
	char c;
	ENABLE_UART_RXINT();
	iziQueueGetFromTask(&iziUartRxQueue,(void*)(&c),IZI_MAX_DELAY);
	return c;
}

//=====================================================================
uint8_t iziUartWriteStream(TIziUartInterface interface, const char *data, uint8_t dataCount)
{
	while(dataCount--)
	{
		iziUartPutchar(eIziUartInterface_0, *(data++));
	}
	return dataCount;
}
//--
uint8_t iziUartReadStream(TIziUartInterface interface, char *data, uint8_t dataCount)
{
	while(dataCount--)
	{
		*(data++)=iziUartGetchar(eIziUartInterface_0);
	}
	return dataCount;
}

//=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
ISR(USART_UDRE_vect)
{
	IziBool_t needYeld = IziFalse;
	char c;

	if(iziQueueGetFromIsr(&iziUartTxQueue,(void *)(&c),&needYeld) == IziTrue) {
		UDR = c;
	} else {
		DISABLE_UART_TXINT();
		if(needYeld == IziTrue)
		{
			iziKernelYeld();
		}
	}
}
//=~=
ISR(USART_RXC_vect)
{
	IziBool_t needYeld = IziFalse;
	char c = UDR;

	iziQueuePutFromIsr(&iziUartRxQueue,(void *)(&c),&needYeld);

	if(needYeld == IziTrue) {
		iziKernelYeld();
	}
}
