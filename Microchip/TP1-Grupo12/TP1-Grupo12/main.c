/*
 * TP1-Grupo12.c
 *
 * Created: 7/4/2026 09:31:21
 * Author : Fedesktop
 */ 

#include <avr/io.h>
#include <avr/cpufunc.h>
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 16MHz
#include <util/delay.h>

#define NOP() __asm__ __volatile__("nop\n\t")
#define NOP_1()  NOP();NOP();NOP();NOP();
#define NOP_2() NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();




static uint8_t np_modo = 0;
static uint8_t np_pos = 0;
static uint8_t np_paridad = 0;


/*
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
*/

// NEOPIXEL SECCION PAAAA

void enviar_byte(uint8_t n){
	for (int i=7; i>=0; i--){
		if((n & (1 << i))){
			PORTB |=  (1 << PORTB0);
			NOP_2();
			PORTB &= ~(1 << PORTB0);
			NOP_1();
		}
		else {
			PORTB |=  (1 << PORTB0);
			NOP_1();
			PORTB &= ~(1 << PORTB0);
			NOP_2();
		}
	}
}

void neopixel(){
	// Modo C
	if(np_modo == 0){	
		np_pos = 0;
		for (int i=0; i<4; i++){
			enviar_byte((255 & (np_paridad)));
			enviar_byte(0);
			enviar_byte(0);	
			
			enviar_byte(0);
			enviar_byte((255 & ~(np_paridad)));
			enviar_byte(0);
		}
		np_paridad = ~np_paridad;
	}
	// Modo D
	else {
		np_paridad = 0;
		for (int i = 0; i < 8; i++){
			enviar_byte(255 * (i == np_pos));
			enviar_byte(0);
			enviar_byte(0);
		}
		np_pos++;
		if (np_pos >= 8) np_pos = 0;
	}
}
// END OF NEOPIXEL SECCION
int main(void)
{
    /* Setup */
	DDRD = 0xFF; // Definimos todo el puerto D como Output
	DDRB |= (1 << PORTB0); // Definimos pin 0 de puerto B como Output
	DDRC &= ~((1<< PORTC0) | (1<< PORTC1)); // Forzamos puertos C como entrada
	PORTC |= ((1<< PORTC0) | (1<< PORTC1)); // Pull up interno para ambas entradas
	
	np_modo = 0;
	np_paridad = 0;
	np_pos = 0;
	 
    while (1) 
    {
		
		neopixel();

		
		// RESET
		PORTB &= ~(1 << PORTB0);
		_delay_ms(150);
		
		
    }
}

