/*
 * TP1-Grupo12.c
 *
 * Created: 7/4/2026 09:31:21
 * Author : Fedesktop
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 16MHz
#include <util/delay.h>



void led_Rebote(uint8_t rebote){
// Desde el LSB hacia el MSB cada 100ms se enciende un led a al vez
	uint8_t izq=1;
	while(1){
		if (rebote == 1){
			if (PORTD & 1 ) izq = 1;	//Si el rebote esta activado, cambia dirección cuando toque extremos
			if (PORTD & 0x80) izq = 0;	
		}else if (PORTD == 0) PORTD = 1; //Si el rebote está desactivado, vuelve a el primer led
		
		_delay_ms(100);
		
		if (izq ==1) PORTD = PORTD << 1;
		}else if (izq == 0) PORTD = PORTD >> 1;
		
	}	
}

int main(void)
{
    /* Setup */
	DDRD = 0xFF; // Definimos todo el puerto D como Output
	DDRB |= (1 << PORTB0); // Definimos pin 0 de puerto B como Output
	//DDRC &= ~((1<< PORTC0) | (1<< PORTC1)) // Forzamos puertos C como entrada
	//PORTC |= ((1<< PORTC0) | (1<< PORTC1)); // Pull up interno para ambas entradas
	
    while (1) 
    {
		PORTD = 0xFF;
		_delay_ms(100);
		PORTD = 0x00;
		_delay_ms(100);
		PORTD = 0x01;
		led_Rebote(0);
	}
}

