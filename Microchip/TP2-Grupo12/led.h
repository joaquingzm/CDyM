/**
 * @file    lede.h
 * @brief   Driver de LEDs indicadores del sistema.
 *
 * @details
 * Provee funciones para inicializar y controlar
 * los LEDs utilizados por el controlador del
 * horno microondas.
 *
 * LEDs utilizados:
 *  - LED Magnetrón:
 *      Indica proceso de cocción activo.
 *
 *  - LED Luz interior:
 *      Indica funcionamiento interno del horno.
 *
 *  - LED Alarma:
 *      Indica finalización del proceso mediante
 *      parpadeo.
 */

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