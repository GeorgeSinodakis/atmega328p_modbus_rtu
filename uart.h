#ifndef uart
#define uart

#include "stdlib.h"

void uart_send_number(uint32_t value,uint8_t format);
void uart_send_number_ln32(uint32_t value,uint8_t format);
void uart_send_number_ln16(uint16_t value,uint8_t format);
void uart_send_number_ln8(uint8_t value,uint8_t format);
void uart_send_string_ln(const char *a);
void uart_send_string(const char *a);
void uart_send_number_binary32(uint32_t value);
void uart_send_number_binary16(uint16_t value);
void uart_send_number_binary8(uint8_t value);
void uart_init(void);

void uart_send_number(uint32_t value,uint8_t format)
{
	char str[11]={};
	ultoa(value,str,format);
	uint8_t i=0;
	while(str[i]!='\0')
	{
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0 = str[i];
		i++;
	}
}

void uart_send_number_ln32(uint32_t value, uint8_t format)
{
	char str[11]={};
	ultoa(value,str,format);
	uint8_t i=0;
	while(str[i]!='\0')
	{
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0 = str[i];
		i++;
	}
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 10;	//new line
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 13;	//carriage return
}

void uart_send_number_ln16(uint16_t value, uint8_t format)
{
	char str[6]={};
	ultoa(value,str,format);
	uint8_t i=0;
	while(str[i]!='\0')
	{
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0 = str[i];
		i++;
	}
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 10;	//new line
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 13;	//carriage return
}

void uart_send_number_ln8(uint8_t value, uint8_t format)
{
	char str[4]={};
	ultoa(value,str,format);
	uint8_t i=0;
	while(str[i]!='\0')
	{
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0 = str[i];
		i++;
	}
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 10;	//new line
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 13;	//carriage return
}

void uart_send_string_ln(const char *a)
{
	while (*a!='\0')
	{
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0=(*a);
		a++;
	}
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 10;	//new line
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = 13;	//carriage return
}

void uart_send_string(const char *a)
{
	while (*a!='\0')
	{
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0=(*a);
		a++;
	}
}

void uart_send_number_binary32(uint32_t value)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = (value & 0xff000000)>>24;
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = (value & 0xff0000)>>16;
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = (value & 0xff00)>>8;
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = value & 0xff;
}

void uart_send_number_binary16(uint16_t value)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = (value & 0xff00)>>8;
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = value & 0x00ff;
}

void uart_send_number_binary8(uint8_t value)
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = value;
}

void uart_init(void)
{
	UCSR0A = 1<<U2X0; //setting uart 8bits,1stop,no parity,2Mbaud
	UCSR0B = 1<<RXEN0|1<<TXEN0;
	UCSR0C = 1<<UCSZ01|1<<UCSZ00;
	UBRR0H = 0;
	UBRR0L = 0;
}

#endif
