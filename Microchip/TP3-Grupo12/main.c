#define F_CPU 16000000UL

#include <avr/io.h>
#include "driver_usart0.h"
#include "terminal.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

int main(void)
{
	terminal_init(F_CPU);
	timer_init();
	sei();
	
	char comando[64];
	char msg[20] = "SEGUNDOS: 0";
	char c = '0';
	while(1)
	{
		terminal_dispatch();
		
		if(FLAG_TIMER)
		{
			FLAG_TIMER = 0;
			
			// TELEMETRIA
			
			// ------ ARMO MSG -------
			c++;
			msg[strlen(msg)-1] = c;
			// ------ FIN ARMADO MSG -----
			
			// ------ MENSAJE ------
			terminal_show_msg(msg);
			// ------ FIN MSJ ------
		}
		
		if(terminal_line_available())
		{
			if(terminal_get_line(comando,sizeof(comando)))
			{
				// algo con comandos
			}
		}
		
		/*if(algo)
		{
			msg = "AYUDAAA NOS VAMOS A MORIR";
		}*/
		
	}
}

