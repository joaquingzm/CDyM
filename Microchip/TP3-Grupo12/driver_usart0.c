/**
 * @file driver_usart0.c
 * @brief -
 */

/*====[Inclusion of own header]==================================*/
#include "driver_usart0.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/*====[Definitions of private global variables]==================*/
// Note: In this buffer implementation, one slot is always left 
// empty to simplify the managment functions and operational logic
#define RX_SIZE 8
#define TX_SIZE 8

static volatile uint8_t rx_buffer[RX_SIZE];
static volatile uint8_t rx_tail;
static volatile uint8_t rx_head;

static volatile uint8_t tx_buffer[TX_SIZE];
static volatile uint8_t tx_tail;
static volatile uint8_t tx_head;

/*====[Prototypes (declarations) of private functions]===========*/
static bool rx_enqueue(uint8_t c);
static bool rx_dequeue(uint8_t *c);
static bool rx_is_empty(void);

static bool tx_enqueue(uint8_t c);
static bool tx_dequeue(uint8_t *c);
static bool tx_is_empty(void);

/*====[Implemenations of public functions]=======================*/
void usart0_init(uint32_t baud_rate, uint32_t f_cpu)
{
	uint16_t ubrr = (f_cpu / (16*baud_rate))-1;
	
	UBRR0H = (uint8_t)ubrr>>8;
	UBRR0L = (uint8_t)ubrr;
	
	RX_OVF_FLAG = false;
	RX_FLAG = false;
	TX_FLAG = false;
	
	UCSR0A &= ~(1<<U2X0);
	
	// 8N1
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	
	// RX + TX enable + RX interrupt
	UCSR0B = (1<<RXEN0)  | (1<<TXEN0) | (1<<RXCIE0);
}

bool usart0_rx_overflow(void)
{
	cli();
	bool f = RX_OVF_FLAG;
	RX_OVF_FLAG = false;
	sei();
	return f;
}

bool usart0_tx_overflow(void)
{
	cli();
	bool f = TX_OVF_FLAG;
	TX_OVF_FLAG = false;
	sei();
	return f;
}

bool usart0_rx_event(void)
{
	cli();
	bool f =  RX_FLAG;
	RX_FLAG = false;
	sei();
	return f;
}

bool usart0_rx_available(void)
{
	cli();
	bool f = rx_is_empty();
	sei();
	return !f;
}

bool usart0_tx_available(void)
{
	cli();
	bool f = tx_is_empty();
	sei();
	return f;
}

bool usart0_read(uint8_t *c)
{
	cli();
	if(rx_is_empty())
	{
		sei();
		return false;
	}
	rx_dequeue(c);
	sei();
	return true;
}

bool usart0_write(uint8_t c)
{
	bool f;
	cli();
	f = tx_enqueue(c);
	if(f)
	{
		UCSR0B |= (1<<UDRIE0);
	}
	sei();
	return f;
}

/*====[Implementations of private functions]=====================*/
static bool rx_enqueue(uint8_t c)
{
	uint8_t next_rx_head = (rx_head + 1) % RX_SIZE;
	
	if(next_rx_head == rx_tail)
	{
		RX_OVF_FLAG = true;
		return false;
	}
	rx_buffer[rx_head] = c;
	rx_head = next_rx_head;	
	return true;
}

static bool rx_dequeue(uint8_t *c)
{
	if(rx_is_empty())
	{
		return false;
	}
	*c = rx_buffer[rx_tail];
	rx_tail = (rx_tail + 1) % RX_SIZE;
	return true;
}

static bool rx_is_empty(void)
{
	return rx_head == rx_tail;
}

static bool tx_enqueue(uint8_t c)
{
	uint8_t next_tx_head = (tx_head + 1) % TX_SIZE;
	
	if(next_tx_head == tx_tail)
	{
		TX_OVF_FLAG = true;
		return false;
	}	
	tx_buffer[tx_head] = c;
	tx_head = next_tx_head;
	return true;
}

static bool tx_dequeue(uint8_t *c)
{	
	if(tx_is_empty())
	{
		return false;
	}
	*c = tx_buffer[tx_tail];
	tx_tail = (tx_tail + 1) % TX_SIZE;
	return true;
}

static bool tx_is_empty(void)
{
	return tx_head == tx_tail;
}

/*====[Implementations of ISRs]==================================*/
ISR(USART_RX_vect)
{
	uint8_t c = UDR0;
	rx_enqueue(c);
	RX_FLAG = 1;
}

ISR(USART_UDRE_vect)
{
	if(!tx_is_empty())
	{
		uint8_t c;
		if(tx_dequeue(&c))
		{
			UDR0 = c;
			TX_FLAG = 1;
		}
	}
	else
	{
		UCSR0B &= ~(1<<UDRIE0);
	}
}

