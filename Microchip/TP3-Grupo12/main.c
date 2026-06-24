#define F_CPU 16000000UL

#include <avr/io.h>
#include "driver_usart0.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	terminal_init();
	sei();
	while(1){
		while (1) 
		{
			terminal_dispatch();
			
		}
	}
}

