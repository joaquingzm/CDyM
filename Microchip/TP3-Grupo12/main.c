/*
 * TP3-Grupo12.c
 *
 * Created: 6/23/2026 14:48:07
 * Author : joaqu
 */ 

#include <avr/io.h>


int main(void)
{
	uart_init();
	
    /* Replace with your application code */
    while (1) 
    {
		if(flag_salto_linea)
		{
			flag_salto_linea := 0;
			terminal_task();
		}
    }
}

