/*
 * TP1-Grupo12.c
 *
 * Created: 7/4/2026 09:31:21
 * Author : Fedesktop
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 16MHz
#include <util/delay.h>



void led_Reboete(){
// Desde el LSB hacia el MSB cada 100ms se enciende un led a al vez
	uint8_t pinEntrada=PIND;
	while(estado1){
				
	}	
}

void led_Desplaazmiento(){
// Desde el LSB hacia el MSB se desplaza repetitivamente

	while(estado2){
		
	}
}




int main(void)
{
    /* Setup */
	DDRD = 0xFF; // Definimos todo el puerto D como Output
	DDRB |= (1 << PORTB0); // Definimos pin 0 de puerto B como Output
	DDRC &= ~((1<< PORTC0) | (1<< PORTC1)) // Forzamos puertos C como entrada
	PORTC |= ((1<< PORTC0) | (1<< PORTC1)) // Pull up interno para ambas entradas
	
    while (1) 
    {
    }
}

