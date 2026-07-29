/*
 * timer1_pwm.c
 */ 

#include "timer1_pwm.h"
#include <avr/io.h>

void timer1_init()
{
	// -- Configuro Compare Output mode, Fast PWM --
	
	//     |    Salida invertida OC1A   |  Salida invertida OC1B    | Bit para FastPWM 8bit |
	TCCR1A |= (1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM10);
	
	//     | Prescaler 256 -> ~244Hz | Bit para FastPWM 8bit |
	TCCR1B |=       (1<<CS12)        | (1<<WGM12);
	
	// -- Configuro salidas --
	DDRB |= (1<<DDB1) | (1<<DDB2);
	
	// Inician apagadas las salidas
	OCR1A = 0; 
	OCR1B = 0;
}

void timer1_pwm_set_duty_A(uint8_t duty)
{
	OCR1A = duty;
}

void timer1_pwm_set_duty_B(uint8_t duty)
{
	OCR1B = duty;
}
