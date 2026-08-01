/*
 * ledRGB.c
 
 Cosas a tener en cuenta de la implementación:
 Para el caso del uso de PWM por Timer1, al haber elegido
 el modo Fast PWM por su simplicidad, se está también contando
 con un comportamiento documentado en la Datasheet (sección 
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
#include "ledRGB.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer1_pwm.h"

/*====[Definitions of private global variables]==================*/
static volatile uint8_t duty_r;

/*====[Implemenations of public functions]=======================*/
void ledRGB_init()
{
	timer1_init();
	
	// -- Configuracion de timer0, Normal--
	TCCR0B |= (1<<CS00);
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
	
	if(count <= duty_r)
	{
		PORTB &= ~(1<<PORTB5);
	}
	else
	{
		PORTB |= (1<<PORTB5);
	}
	count++;
}
