/*
 * TP1-Grupo12.c
 *
 * Created: 7/4/2026 09:31:21
 * Author : Fedesktop
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL // Defino la frecuencia de oscilador en 16MHz
#include <util/delay.h>
#define LEDS_ITERATION 2

uint8_t botonPresionado(uint8_t num){
	if (!(PINC & (1<<num))) return 1;
	else return 0;
}


void cambiarLeds(uint8_t *leds, uint8_t rebote, uint8_t *izq){
	if (*leds == 0x01){
		if (rebote){
			*izq = 1;
			*leds <<=1;
			return;
		}
	}
	
	if (*leds == 0x80){
		if (rebote){
			*izq = 0;
			*leds >>= 1;
		}else {
			*leds = 0x01;
		}
		return;
	}
	
	if (*izq) *leds <<= 1;
	else *leds >>= 1;
	return;
}

void setup(){
	/* Setup */
	DDRD = 0xFF; // Definimos todo el puerto D como Output
	DDRB |= (1 << PORTB0); // Definimos pin 0 de puerto B como Output
	DDRC &= ~((1<< PORTC0) | (1<< PORTC1)); // Forzamos puertos C como entrada
	PORTC |= ((1<< PORTC0) | (1<< PORTC1)); // Pull up interno para ambas entradas
}

int main(void){
    setup();
	
	/*Botones*/
	uint8_t B1;
	uint8_t B1_prev = 0;
	//uint8_t B2;
	
	
	/*variables Leds*/
	uint8_t leds=0x01;
	uint8_t rebote=0;
	uint8_t izq=1;
	uint8_t ledsIteration=0;
	
	/*Variables Neopixel*/
	
	PORTD = leds;
    while (1) 
    {
		if (ledsIteration>=LEDS_ITERATION){
			cambiarLeds(&leds,rebote,&izq);
			PORTD = leds;
			ledsIteration=0;
		}
		
		/*
		if ((neoIteration-NEO_ITERATION)==0){
			cambiarNeo();
			neoIteration=0;
		}
		*/
		
		B1 = botonPresionado(PINC0);
		if (B1_prev && !B1){
			rebote= rebote^1;
			
			if (rebote){
				leds = 0x80;
				izq = 0;
			}else {
				leds = 0x01;
				izq = 1;
			}
			
			PORTD = leds;
			ledsIteration = 0;
		}
		B1_prev=B1;
		
		ledsIteration++;
		//neoIteration++;
		_delay_ms(50);
	}
}


