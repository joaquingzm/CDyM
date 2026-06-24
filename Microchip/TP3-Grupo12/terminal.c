/*
#define TERM_TX_SIZE

#include "driver_usart0.h"

static uint8_t term_tx_buffer[TERM_TX_SIZE];
static uint8_t term_tx_head = 0;
static uint8_t term_tx_tail = 0;
static bool term_tx_pending = false;

void terminal_init();

void terminal_write(const char *s)
{
	while(*s)
	{
		term_tx_buffer[term_tx_head] = *s++;
		term_tx_head = (term_tx_head + 1) % TERM_TX_SIZE; 
	}
	term_tx_pending = true;
}


void terminal_dispatch()
{
	if(usart0_rx_event())
	{
		while(usart0_rx_available())
		{
			uint8_t c = usart0_read();
			terminal_process_char(c);
		}
	}	
	
	if(term_tx_pending)
	{
		if(!term_tx_is_empty())
		{
			uint8_t c = term_tx_buffer[term_tx_tail];
			if(usart0_write(c))
			{
				term_tx_tail = (term_tx_tail + 1) % TERM_TX_SIZE;
			}
		}
		else
		{
			term_tx_pending = false;
		}
	}
}


*/