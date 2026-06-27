#define F_CPU 16000000UL

#include <avr/io.h>
#include "driver_usart0.h"
#include "terminal.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
//añado pruebas del sensor
#include "driver_dht11.h"

int main(void)
{
	terminal_init(F_CPU,"H");
	timer_init();
	sei();
	
	//char comando[64];
	char msg[64] = " ";
	//char c = '0';
	while(1)
	{
		terminal_dispatch();
		//msg[0]='a';
		if(FLAG_TIMER)
		{
			FLAG_TIMER = 0;
			//msg[0]=msg[0]+1;
			
			// TELEMETRIA
			
			// ------ ARMO MSG -------
			dht11_data_t data_dht11;
			dht11_status_t status_dht11;
			status_dht11 = dht11_read(&data_dht11);
			if (status_dht11 == DHT11_ERR_NO_RESPONSE){
				strcpy(msg, "ERROR DE SENSOR: NO RESPONSE");
			}
			
			if (status_dht11 == DHT11_ERR_CHECKSUM){
				strcpy(msg, "ERROR DE SENSOR: CHECKSUM INCORRECTO");
			}
			
			if (status_dht11 == DHT11_ERR_TIMEOUT){
				strcpy(msg, "ERROR DE SENSOR: TIMEOUT");
			}
			
			if(status_dht11 == DHT11_OK){
				char t1,t2,h1,h2;
				t1 = (data_dht11.temperature_int / 10) + '0'; //decena
				t2 = (data_dht11.temperature_int % 10) + '0'; //unidad 
				h1 = (data_dht11.humidity_int / 10) + '0';
				h2 = (data_dht11.humidity_int % 10) + '0';
				
				msg[0] = 'T';
				msg[1] = '=';
				msg[2] = t1;
				msg[3] = t2;
				msg[4] = '|';
				msg[5] = 'H';
				msg[6] = '=';
				msg[7] = h1;
				msg[8] = h2;
			}
			// ------ FIN ARMADO MSG -----
			
			// ------ MENSAJE ------
			terminal_show_msg(msg);
			// ------ FIN MSJ ------
		}
		
		if(terminal_line_available())
		{
			if(terminal_get_line(msg,sizeof(msg)))
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

