/*
 * timer2.c
 */ 

/*====[Inclusion of own header]==================================*/
#include "timer2.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>
#include <avr/interrupt.h>

/*====[Implemenations of public functions]=======================*/
void timer2_init(void){
	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS22);
	OCR2A = 249;
	TIMSK2 = (1 << OCIE2A);
}

/*====[Implementations of private functions]=====================*/
ISR (TIMER2_COMPA_vect){
	static uint16_t count = 0;
	if (++count >= 10){
		count = 0;
		FLAG_TIMER = 1;
	}
}
