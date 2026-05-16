/***********************************************************************
* Archivo: led.h
* Descripción:
*
*
***********************************************************************/

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _LED_
#define _LED_

/*====[Prototypes (declarations) of public functions]============*/
void led_init(void);

void led_magnetron_on(void);
void led_magnetron_off(void);

void led_light_on(void);
void led_light_off(void);

void led_alarm_on(void);
void led_alarm_off(void);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _LED_ */