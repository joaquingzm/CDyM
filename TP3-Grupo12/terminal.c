/**
 * @file driver_usart0.c
 * @brief -
 */

/*====[Inclusion of own header]==================================*/
#include "terminal.h"

/*====[Inclusion of private function dependencies]===============*/
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "driver_usart0.h"

/*====[Definitions of private global variables]==================*/
#define RX_ASSEMBLY_SIZE 20
#define MSG_SIZE 100
#define TX_SIZE ((RX_ASSEMBLY_SIZE + MSG_SIZE) * 2)
#define PROMPT "CMD> "
#define PROMPT_LEN 5

// FIFO 
static char tx_buffer[TX_SIZE];
static uint8_t tx_head = 0;
static uint8_t tx_tail = 0;
static bool tx_pending = false;
static bool TERM_TX_OVF_FLAG = false;

// LIFO 
static char rx_assembly[RX_ASSEMBLY_SIZE]; 
static uint8_t rx_assembly_top = 0;
static bool TERM_RX_ASSEMBLY_OVF = false;

// Buffer for 1 line
static char rx_line_buf[RX_ASSEMBLY_SIZE]; 
static bool rx_line_ready = false;
static bool TERM_RX_LINE_OVF = false;  

// Buffer
static char message[MSG_SIZE];

/*====[Prototypes (declarations) of private functions]===========*/
static void terminal_process_char(char c);
static void terminal_redraw(void);
static void terminal_clear_all(void);
static uint8_t terminal_send(char *s);

static bool tx_enqueue(char c);
static bool tx_peak(char *c);
static bool tx_dequeue(char *c);
static bool tx_is_empty(void);

static bool rx_assembly_push(char c);
static bool rx_assembly_pop(char *c);
static bool rx_assembly_is_empty(void); 

/*====[Implemenations of public functions]=======================*/
void terminal_init(uint32_t f_cpu,char *initial_msg)
{
	usart0_init(9600,f_cpu);
	terminal_show_msg(initial_msg);
}

void terminal_dispatch(void)
{
	
	while(usart0_rx_available())
	{
		uint8_t c;
		if(usart0_read(&c))
		{ 
			terminal_process_char((char)c);  // echo decided by process_char
		}
	}	
	
	if(tx_pending)
	{
		char c;
		while((!tx_is_empty()))
		{
			tx_peak(&c);
			if(!usart0_write((uint8_t)c))
			{
				break;
			}
			tx_dequeue(&c);	
		}
		if(tx_is_empty())
		{
			tx_pending = false;
		}
	}
}

uint8_t terminal_show_msg(char *msg)
{
	terminal_clear_all();  
	
	strncpy(message, msg, sizeof(message));
	message[sizeof(message) - 1] = '\0';

	terminal_redraw();
	
	return 1;
}

bool terminal_line_available(void)
{
	return rx_line_ready;
}

bool terminal_get_line(char *buf, uint8_t max_len)
{
	if(!rx_line_ready || max_len == 0 || buf == NULL) return false;
	
	uint8_t line_len = strlen(rx_line_buf);
	
	if(max_len < line_len)
	{
		rx_line_ready = false;
		buf[0] = '\0';
		return false;
	}
	
	strcpy(buf,rx_line_buf);
	rx_line_ready = false;
	return true;
}

bool terminal_rx_overflow(void)
{
	bool ovf = TERM_RX_LINE_OVF;
	TERM_RX_LINE_OVF = false;
	return ovf;
}

/*====[Implementations of private functions]=====================*/
static void terminal_process_char(char c)
{
	char aux;
	
	switch(c)
	{
		case '\r':
			if(rx_assembly_is_empty()) break;
			
			rx_assembly[rx_assembly_top] = '\0';
			
			if(!rx_line_ready)
			{
				strcpy(rx_line_buf, rx_assembly);
				rx_line_ready = true;
			}
			else
			{
				TERM_RX_LINE_OVF = true;
			}
			
			terminal_clear_all();      
			rx_assembly_top = 0;
			terminal_redraw();
			
			break;
		
		case '\b':
			if(!rx_assembly_is_empty())
			{
				rx_assembly_pop(&aux);
				//tx_enqueue('\b');      
				//tx_pending = true;
			}
			break;
		case '\n':
			break;
		
		default:
			if(rx_assembly_top < RX_ASSEMBLY_SIZE - 1)
			{
				rx_assembly_push(c);
				//tx_enqueue(c);        
				//tx_pending = true;
			}
			else
			{
				TERM_RX_LINE_OVF = true;
			}
			break;
	}
}

static void terminal_clear_all(void)
{
	uint8_t total = strlen(message) + PROMPT_LEN + rx_assembly_top;
	for(uint8_t i = 0; i < total+1; i++)
		tx_enqueue('\b');
	tx_pending = true;
}

static void terminal_redraw(void)
{
	terminal_send(message);
	terminal_send("\r");
	terminal_send(PROMPT);
	for(uint8_t i = 0; i < rx_assembly_top; i++)
		tx_enqueue(rx_assembly[i]);
	tx_pending = true;
}

static uint8_t terminal_send(char *s)
{
	uint8_t cont = 0;
	
	while(*s)
	{
		if(!tx_enqueue(*s++))
		{
			return cont;
		}
		cont++;
	}
	
	if(!tx_is_empty()) tx_pending = true;
	
	return cont;
}

static bool tx_enqueue(char c)
{
	uint8_t next_tx_head = (tx_head + 1) % TX_SIZE;
	
	if(next_tx_head == tx_tail)
	{
		TERM_TX_OVF_FLAG = true;
		return false;
	}
	tx_buffer[tx_head] = c;
	tx_head = next_tx_head;
	return true;
}

static bool tx_dequeue(char *c)
{
	if(tx_is_empty())
	{
		return false;
	}
	*c = tx_buffer[tx_tail];
	tx_tail = (tx_tail + 1) % TX_SIZE;
	return true;
}

static bool tx_peak(char *c)
{
	if(tx_is_empty())
	{
		return false;
	}
	*c = tx_buffer[tx_tail];
	return true;
}

static bool tx_is_empty(void)
{
	return tx_head == tx_tail;
}

static bool rx_assembly_push(char c)
{
	if(rx_assembly_top == RX_ASSEMBLY_SIZE)
	{
		TERM_RX_ASSEMBLY_OVF = true;
		return false;
	}
	
	rx_assembly[rx_assembly_top] = c;
	rx_assembly_top++;
	return true;
}

static bool rx_assembly_pop(char *c)
{
	if(rx_assembly_is_empty())
	{
		return false;
	}
	rx_assembly_top--;
	*c = rx_assembly[rx_assembly_top];
	return true;
}

static bool rx_assembly_is_empty(void)
{
	return rx_assembly_top == 0;
}