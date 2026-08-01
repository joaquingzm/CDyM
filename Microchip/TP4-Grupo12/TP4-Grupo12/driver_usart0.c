/**
 * @file driver_usart0.c
 * @brief   Driver for USART0 peripheral of the ATmega328P.
 *
 * @details
 * Implementation with two independent ring buffers :
 *   - rx_buffer: filled by ISR(USART_RX_vect), consumed by usart0_read().
 *   - tx_buffer: filled by usart0_write(), consumed by ISR(USART_UDRE_vect).
 *
 * In both buffers implementation, one slot is always left
 * empty to simplify the managment functions and operational logic
 *   - Empty: head == tail
 *   - Full: (head + 1) % SIZE == tail
 *   - Real capacity: SIZE - 1 bytes
 *
 * Flags are internal variables. They can be accesed only through public
 * functions, which guarantees atomicity with cli/sei.
 */

/*====[Inclusion of own header]==================================*/
#include "driver_usart0.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

/*====[Definitions of private global variables]==================*/
#define RX_SIZE 16
#define TX_SIZE 128

/* Reception buffer: Filled by ISR, consumed by usart0_read() */
static volatile uint8_t rx_buffer[RX_SIZE];
static volatile uint8_t rx_tail;
static volatile uint8_t rx_head;

/* Transmission buffer: Filled by usart0_write(), consumed by ISR */
static volatile uint8_t tx_buffer[TX_SIZE];
static volatile uint8_t tx_tail;
static volatile uint8_t tx_head;

/* Event flags: Written by ISRs, read and cleared by public functions */
static volatile bool rx_flag     = false;   
static volatile bool rx_ovf_flag = false;   
static volatile bool tx_ovf_flag = false;   


/*====[Prototypes (declarations) of private functions]===========*/
static bool rx_enqueue(uint8_t c);
static bool rx_dequeue(uint8_t *c);
static bool rx_is_empty(void);

static bool tx_enqueue(uint8_t c);
static bool tx_dequeue(uint8_t *c);
static bool tx_is_empty(void);
static bool tx_is_full(void);

/*====[Implemenations of public functions]=======================*/
void usart0_init(uint32_t baud_rate, uint32_t f_cpu)
{
	
	uint16_t ubrr = (uint16_t)USART0_UBRR(baud_rate, f_cpu);
	    
	UCSR0A = 0x00;                              /* U2X0 = 0: normal mode      */
	UBRR0H = (uint8_t)(ubrr >> 8);             /* Bits [11:8] baud rate  */
	UBRR0L = (uint8_t)(ubrr);                  /* Bits [7:0]  baud rate  */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);        /* 8 bits, no parity, 1 stop */
	    
	/* Driver internal state reset */
	rx_head     = 0;
	rx_tail     = 0;
	tx_head     = 0;
	tx_tail     = 0;
	rx_flag     = false;
	rx_ovf_flag = false;
	tx_ovf_flag = false;
	    
	/* Enable peripheral: RX + TX + RX interruption */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);

}

bool usart0_rx_overflow(void)
{
	cli();
	bool f      = rx_ovf_flag;
	rx_ovf_flag = false;
	sei();
	return f;
}

bool usart0_tx_overflow(void)
{
	cli();
	bool f      = tx_ovf_flag;
	tx_ovf_flag = false;
	sei();
	return f;
}

bool usart0_rx_event(void)
{
    cli();
    bool f  = rx_flag;
    rx_flag = false;
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
	cli();
	bool f = tx_enqueue(c);
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
		rx_ovf_flag = true;
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
		tx_ovf_flag = true;
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

static bool tx_is_full(void)
{
	return ((tx_head + 1) % TX_SIZE) == tx_tail;
}

/*====[Implementations of ISRs]==================================*/

/**
 * @brief ISR for USART reception.
 *		  UDR0 must always be read (even if rx_buffer is full)
 *		  so the hardware flag can be cleared in order to not block 
 *		  future receptions.
 */

ISR(USART_RX_vect)
{
	uint8_t c = UDR0;
	rx_enqueue(c);
	rx_flag = true;
}

/**
 * @brief ISR for empty data register (UDRE).
 *		  Its called when UDR0 is ready to receive the next byte.
 *		  If TX buffer is empty, the interruption gets disabled so 
 *		  it doesnt keeps calling.
 */
ISR(USART_UDRE_vect)
{
	uint8_t c;
	if(tx_dequeue(&c))
	{
		UDR0 = c;
	}
	else
	{
		UCSR0B &= ~(1<<UDRIE0);
	}
}

