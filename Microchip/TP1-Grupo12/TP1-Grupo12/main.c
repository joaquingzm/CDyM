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
#define LEDS_ITERATION 20 //100ms = leds_iteration * Refresh_rate_ms 
#define NEO_ITERATION 30 // 150ms = Neo_iteration * Refresh_rate_ms
#define REFRESH_RATE_MS 5

<<<<<<< HEAD
uint8_t botonPresionado(uint8_t num){
	if (!(PINC & (1<<num))) return 1;
	else return 0;
}


void cambiarLeds(uint8_t *leds, uint8_t rebote, uint8_t *izq){
	if (*leds == 0x01){	//Extremo derecho
			*izq = 1;
			*leds <<=1;
			return;
	}
	if (*leds == 0x80){	//Extremo izquierdo
		if (rebote){
			*izq = 0;
			*leds >>= 1;
		}else {
			*leds = 0x01;
		}
		return;
	}
	if (*izq) *leds <<= 1;	// Desplazar a Izquierda o Derecha
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
		_delay_ms(REFRESH_RATE_MS);
	}
}
=======
#define NOP() __asm__ __volatile__("nop\n\t")

#define LEDS_ITERATION 20 //100ms = leds_iteration * Refresh_rate_ms
#define NEO_ITERATION 30 // 150ms = Neo_iteration * Refresh_rate_ms
#define REFRESH_RATE_MS 5

// Variables Neopixel
static uint8_t np_modo = 0;
static uint8_t np_pos = 0;
static uint8_t np_paridad = 0;
static uint8_t np_Iteration = 0;

// Variables leds
static uint8_t ld_modo = 0;
static uint8_t ld_leds = 0x01;
static uint8_t ld_izq = 1;
static uint8_t ld_Iteration = 0;
>>>>>>> da78821137920954af6dfc0c75ef63b5c48cde13

// --- NEOPIXEL
void enviar_byte(uint8_t n);
void neopixel();
// END OF NEOPIXEL 

<<<<<<< HEAD
=======
// LEDS
void cambiarLeds();
// END OF LEDS

void setup();
uint8_t botonPresionado(uint8_t n);

int main(void)
{
	setup();
	
	/*Botones*/
	uint8_t B1;
	uint8_t B1_prev = 0;
	uint8_t B2;
	uint8_t B2_prev = 0;
	
	PORTD = ld_leds;
	 
    while (1) 
    {
		
		if (ld_Iteration>=LEDS_ITERATION){
			cambiarLeds();
			ld_Iteration=0;
		}
		
		if (np_Iteration>=NEO_ITERATION){
			neopixel();
			np_Iteration=0;
		}
		
		
		B1 = botonPresionado(PINC0);
		if (B1_prev && !B1){
			ld_modo= ld_modo^1;
			
			if (ld_modo){
				ld_leds = 0x80;
				ld_izq = 0;
			}else {
				ld_leds = 0x01;
				ld_izq = 1;
			}
			
			PORTD = ld_leds;
			ld_Iteration = 0;
		}
		B1_prev=B1;
		
		B2 = botonPresionado(PINC1);
		if (B2_prev && !B2){
			np_modo= np_modo^1;
			
			if (np_modo){
				np_paridad = 0;
			}else {
				np_pos = 0;
			}
			
			np_Iteration = 0;
		}
		B2_prev=B2;
		
		ld_Iteration++;
		np_Iteration++;
		PORTD = ld_leds;
		_delay_ms(REFRESH_RATE_MS);
	}
		
}

void setup(){
	/* Setup */
	DDRD = 0xFF; // Definimos todo el puerto D como Output
	DDRB |= (1 << PORTB0); // Definimos pin 0 de puerto B como Output
	DDRC &= ~((1<< PORTC0) | (1<< PORTC1)); // Forzamos puertos C como entrada
	PORTC |= ((1<< PORTC0) | (1<< PORTC1)); // Pull up interno para ambas entradas

	np_modo = 0;
	np_paridad = 0;
	np_pos = 0;
}

uint8_t botonPresionado(uint8_t n){
	if (!(PINC & (1<<n))) return 1;
	else return 0;
}

// --- NEOPIXEL
void enviar_byte(uint8_t n){
	for (uint8_t i=0; i<8; i++){ // Cada iteración: Suma 5 Ciclos ->
		// Última iteración:  Suma 4 Ciclos
		
		// BIT 1
		if(n & 0x80){ // Suma ~3 Ciclos (Si bit 1 -> 2 Ciclos, si bit 0 -> 3 Ciclos
			
			// T1H = 12 Ciclos -> 750ns
			// T1L = 9 Ciclos -> 562,5ns
			
			PORTB |=  (1 << PORTB0); // Suma 3 Ciclos -> 187,5ns
			NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP(); // Suma 9 Ciclos -> 562,5ns
			PORTB &= ~(1 << PORTB0); // Suma 3 Ciclos -> 187,5ns
			
		}
		
		// BIT 0
		else {
			
			// T0H = 6 Ciclos -> 375ns
			// T0L = 14 Ciclos -> 875ns
			
			PORTB |=  (1 << PORTB0); // Suma 3 Ciclos -> 187,5ns
			NOP();NOP();NOP(); // Suma 3 Ciclos -> 187,5ns
			PORTB &= ~(1 << PORTB0); // Suma 3 Ciclos -> 187,5ns
			NOP();NOP();NOP();NOP();NOP(); // Suma 5  Ciclos -> 312,5ns
			
		}
		n <<=1; // Suma 1 Ciclo -> 62,5ns
	}
}

void neopixel(){
	// Modo C
	if(np_modo == 0){
		for (int i=0; i<4; i++){
			enviar_byte(0);
			enviar_byte(0);
			enviar_byte((255 & (np_paridad)));
			
			enviar_byte(0);
			enviar_byte((255 & ~(np_paridad)));
			enviar_byte(0);
		}
		np_paridad = ~np_paridad;
	}
	// Modo D
	else {
		for (int i = 0; i < 8; i++){
			enviar_byte(255 * (i == 8 - np_pos));
			enviar_byte(0);
			enviar_byte(0);
		}
		np_pos++;
		if (np_pos > 8) np_pos = 0;
	}
}
// END OF NEOPIXEL


// LEDS
void cambiarLeds(){
	if (ld_leds == 0x01){	//Extremo derecho
		ld_izq = 1;
		ld_leds <<=1;
		return;
	}
	if (ld_leds == 0x80){	//Extremo izquierdo
		if (ld_modo){
			ld_izq = 0;
			ld_leds >>= 1;
		}else {
			ld_leds = 0x01;
		}
		return;
	}
	if (ld_izq) ld_leds <<= 1;	// Desplazar a Izquierda o Derecha
	else ld_leds >>= 1;
	return;
}
// END OF LEDS
>>>>>>> da78821137920954af6dfc0c75ef63b5c48cde13
