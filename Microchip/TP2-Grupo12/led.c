/**
 * @file led.c
 * @brief Implementación de driver de LEDs indicadores del sistema.
 */
/*====[Inclusion of own header]==================================*/
#include "led.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>

/*====[Implemenations of public functions]=======================*/
void led_init(void){
	DDRB |= (1 << PORTB5);
	DDRC |= (1 << PORTC4);
	DDRC |= (1 << PORTC5);
	
	led_magnetron_off();
	led_light_off();
	led_alarm_off();
}

/***********************************************************************
* Prop?sito:
*   Enciende LED del magnetr?n.
***********************************************************************/
void led_magnetron_on(void)
{
	PORTB |= (1 << PORTB5);
}

/***********************************************************************
* Prop?sito:
*   Apaga LED del magnetr?n.
***********************************************************************/
void led_magnetron_off(void)
{
	PORTB &= ~(1 << PORTB5);
}

/***********************************************************************
* Prop?sito:
*   Enciende LED de luz interior.
***********************************************************************/
void led_light_on(void)
{
	PORTC |= (1 << PORTC4);
}

/***********************************************************************
* Prop?sito:
*   Apaga LED de luz interior.
***********************************************************************/
void led_light_off(void)
{
	PORTC &= ~(1 << PORTC4);
}

/***********************************************************************
* Prop?sito:
*   Enciende LED de alarma.
***********************************************************************/
void led_alarm_on(void)
{
	PORTC |= (1 << PORTC5);
}

/***********************************************************************
* Prop?sito:
*   Apaga LED de alarma.
***********************************************************************/
void led_alarm_off(void)
{
	PORTC &= ~(1 << PORTC5);
}