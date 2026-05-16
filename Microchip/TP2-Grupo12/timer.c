/*====[Inclusion of own header]==================================*/
#include "timer.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>
#include <avr/interrupt.h>

/*====[Implemenations of public functions]=======================*/
void timer_init(void){
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS01) | (1<< CS00);
	OCR0A = 249;
	TIMSK0 = (1 << OCIE0A);
	sei();
}

/*====[Implementations of private functions]=====================*/
ISR (TIMER0_COMPA_vect){
	static uint8_t count = 0;
	if (++count >= 100){
		count = 0;
		FLAG_TIMER = 1;
	}
}
