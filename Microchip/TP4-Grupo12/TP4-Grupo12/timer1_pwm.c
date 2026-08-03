/*
 * timer1_pwm.c
 
  Cosas a tener en cuenta de la implementación:
  Para el modo Fast PWM, se da un
  comportamiento documentado en la Datasheet (sección
  Fast PWM de Timer 1, 16.9.3), el cual es que siempre que OCRx
  está en 0, para cada vez que el contador llega a BOTTOM, durante
  un ciclo de reloj se produce un pico, esto es inherente a la
  implementación de Timer 1.
  
  'The extreme values for the OCR1x Register represents special cases when generating a PWM
  waveform output in the fast PWM mode. If the OCR1x is set equal to BOTTOM (0x0000) the out-
  put will be a narrow spike for each TOP+1 timer clock cycle. Setting the OCR1x equal to TOP
  will result in a constant high or low output (depending on the polarity of the output set by the
  COM1x1:0 bits.)'
 
 */ 

/*====[Inclusion of own header]==================================*/
#include "timer1_pwm.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>

/*====[Implemenations of public functions]=======================*/
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
	/*
	if (duty == 0)
	{
		// Desconecto el Timer1 del pin OC1A 
		TCCR1A &= ~((1<<COM1A1) | (1<<COM1A0));
		// Fuerzo el pin a ALTO para apagar el LED (por la lógica invertida)
		PORTB |= (1<<PORTB1);
	}
	else
	{
		// Reconecto el Timer1 al pin OC1A en modo invertido
		TCCR1A |= (1<<COM1A1) | (1<<COM1A0);
	}*/
}

void timer1_pwm_set_duty_B(uint8_t duty)
{
	OCR1B = duty;
	/*
	if (duty == 0)
	{
		// Desconecto el Timer1 del pin OC1B (Limpia los bits COM)
		TCCR1A &= ~((1<<COM1B1) | (1<<COM1B0));
		// Fuerzo el pin a ALTO para apagar el LED (por la lógica invertida)
		PORTB |= (1<<PORTB2);
	}
	else
	{
		// Reconecto el Timer1 al pin OC1B en modo invertido
		TCCR1A |= (1<<COM1B1) | (1<<COM1B0);
	}*/
}
