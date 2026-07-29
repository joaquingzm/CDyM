/*
 * ledRGB.c
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "ledRGB.h"
#include "timer1_pwm.h"

static volatile uint8_t duty_r;

void ledRGB_init()
{
	timer1_init();
	
	// -- Configuracion de timer0, Normal--
	// Prescaler 8 -> ~30.5Hz > 30Hz
	TCCR0B |= (1<<CS01);
	TIMSK0 |= (1<<TOIE0);
	
	DDRB |= (1<<DDB5);
	PORTB |= (1<<PORTB5);
	
	
}

void ledRGB_set(uint8_t r, uint8_t g, uint8_t b)
{
	duty_r = r;
	timer1_pwm_set_duty_B(g);
	timer1_pwm_set_duty_A(b);
}


// Como la interrupcion de ISR ocurre a una frencuencia de fclk/N.256 
// --> entonces la frecuencia de PWM por software es cuando count completa
// un ciclo (256 cuentas), por lo tanto fpwm_soft = fclk/N.256.256. Con N=1 
// da ~ 244Hz que supera los 30Hz del enunciado e iguala la frecuencia de PWM
// de como está configurado timer1 en este caso
ISR(TIMER0_OVF_vect)
{
	static uint8_t count = 0;
	
	if(count < duty_r)
	{
		PORTB &= ~(1<<PORTB5);
	}
	else
	{
		PORTB |= (1<<PORTB5);
	}
	count++;
}
