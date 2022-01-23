#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "modbus.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

volatile modbusServer modbusAVR;

ISR(USART_RX_vect)
{
	if (!modbusAVR.requestPending)
	{
		TCNT0 = 0;
		modbusAVR.messageBuffer[modbusAVR.messageIndex++] = UDR0;
		if (modbusAVR.messageIndex >= sizeof(modbusAVR.messageBuffer))
		{
			modbusAVR.messageIndex = 0;
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	if (!modbusAVR.requestPending)
	{
		if (modbusAVR.messageIndex < 8) // we have junk
		{
			modbusAVR.messageIndex = 0;
		}
		else
		{
			modbusAVR.requestPending = 1;
		}
	}
}

int main(void)
{
	// PORT init
	DDRB = 1;

	// uart init
	UCSR0A = 1 << U2X0;
	UCSR0B = 1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0;
	UCSR0C = 1 << UCSZ01 | 1 << UCSZ00;
	UBRR0H = 0; // 19200 baudrate
	UBRR0L = 103;

	// timer0 init
	TCNT0 = 0;
	OCR0A = 23;
	TIMSK0 = 1 << OCIE0A;
	TCCR0A = 1 << WGM01;
	TCCR0B = 1 << CS02 | 1 << CS00; // counter > 23 --> new message

	modbusAVR.HoldingRegisters[0] = 1;
	modbusAVR.id = ID;
	modbusAVR.messageIndex = 0;
	modbusAVR.requestPending = 0;

	sei();

	while (1)
	{
		if (modbusAVR.requestPending)
		{
			modbusHandleRequest(&modbusAVR);

			UDR0; // clear pending interrupts
		}
		
		if(modbusAVR.HoldingRegisters[0] == 20)
			modbusAVR.HoldingRegisters[0] = 0;
		else
			modbusAVR.HoldingRegisters[0]++;
		_delay_ms(500);
	}
}
