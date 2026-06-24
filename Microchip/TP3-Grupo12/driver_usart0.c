/**
 * @file driver_usart0.c
 * @brief -
 */

/*====[Inclusion of own header]==================================*/
#include "driver_usart0.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>
#include <avr/interrupt.h>

/*====[Definitions of private global variables]==================*/
#define RX_SIZE 8
#define TX_SIZE 4

static  uint8_t rx_buffer[RX_SIZE];
static volatile uint8_t rx_tail = 0;
static volatile uint8_t rx_head = 0;

static  uint8_t tx_buffer[TX_SIZE];
static volatile uint8_t tx_tail = 0;
static volatile uint8_t tx_head = 0;

static bool USART0_RX_OVF_FLAG;
static bool USART0_RX_FLAG;
static bool USART0_TX_FLAG;

/*====[Prototypes (declarations) of private functions]===========*/
static bool rx_push(uint8_t c);
static uint8_t rx_pop(void);
static bool rx_is_empty(void);
static bool tx_push(uint8_t c);
static uint8_t tx_pop(void);
static bool tx_is_empty(void);

/*====[Implemenations of public functions]=======================*/

void usart0_init(uint32_t baud_rate, uint32_t f_cpu)
{
	uint16_t ubrr = (f_cpu / (16*baud_rate))-1;
	
	UBRR0H = (uint8_t)ubrr>>8;
	UBRR0L = (uint8_t)ubrr;
	
	USART0_RX_OVF_FLAG = false;
	USART0_RX_FLAG = false;
	USART0_TX_FLAG = false;
	
	UCSR0A &= ~(1<<U2X0);
	
	// 8N1
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	
	// RX + TX enable + RX interrupt
	UCSR0B = (1<<RXEN0)  | (1<<TXEN0) | (1<<RXCIE0);
}

bool usart0_rx_overflow(void)
{
	cli();
	bool f = USART0_RX_OVF_FLAG;
	USART0_RX_OVF_FLAG = false;
	sei();
	return f;
}

bool usart0_rx_event(void)
{
	cli();
	bool f =  USART0_RX_FLAG;
	USART0_RX_FLAG = false;
	sei();
	return f;
}

bool usart0_rx_available(void)
{
	return !rx_is_empty();
}

uint8_t usart0_read()
{
	if(rx_is_empty())
	{
		// hacer algo;
		return ' ';
	}
	
	return rx_pop();
}

bool usart0_write(uint8_t c)
{
	bool f;
	cli();
	f = tx_push(c);
	if(f)
	{
		UCSR0B |= (1<<UDRIE0);
	}
	sei();
	return f;
}

/*====[Implementations of private functions]=====================*/

static bool rx_push(uint8_t c)
{
	uint8_t next_rx_head = (rx_head + 1) % RX_SIZE;
	
	if(next_rx_head == rx_tail)
	{
		USART0_RX_OVF_FLAG = 1;
		return false;
	}
	rx_buffer[rx_head] = c;
	rx_head = next_rx_head;	
	return true;
}

static uint8_t rx_pop()
{
	uint8_t c = rx_buffer[rx_tail];
	rx_tail = (rx_tail + 1) % RX_SIZE;
	return c;
}

static bool rx_is_empty()
{
	return rx_head == rx_tail;
}

static bool tx_push(uint8_t c)
{
	uint8_t next_tx_head = (tx_head + 1) % TX_SIZE;
	
	if(next_tx_head == tx_tail)
	{
		return false;
	}	
	tx_buffer[tx_head] = c;
	tx_head = next_tx_head;
	return true;
}

static uint8_t tx_pop()
{
	uint8_t c = tx_buffer[tx_tail];
	tx_tail = (tx_tail + 1) % TX_SIZE;
	return c;
}

static bool tx_is_empty()
{
	return tx_head == tx_tail;
}

/*====[Implementations of ISRs]==================================*/

ISR(USART_RX_vect)
{
	uint8_t c = UDR0;
	rx_push(c);
	USART0_RX_FLAG = 1;
}

ISR(USART_UDRE_vect)
{
	if(!tx_is_empty())
	{
		UDR0 = tx_pop();
		USART0_TX_FLAG = 1;
	}
	else
	{
		UCSR0B &= ~(1<<UDRIE0);
	}
}

