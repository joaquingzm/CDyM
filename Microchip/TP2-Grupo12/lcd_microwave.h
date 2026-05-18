/**
 * @file    lcd_microwave.h
 * @brief   Driver LCD.
 *
 * @details
 * Provee funciones para inicializar y controlar
 * un display LCD.
 *
 * El módulo permite:
 * - mostrar tiempo en pantalla
 * - mostrar mensajes en pantalla
 * - alternar entre encendido y apagado
 * - ALGO SOBRE LOADING?
 *
 */

/*=====[Avoid multiple inclusion - begin]===========================*/
#ifndef _LCD_MICROWAVE_H_
#define _LCD_MICROWAVE_H_

/*=====[Inclusions of public function dependencies]=================*/
#include <stdint.h>

/*=====[Prototypes (declarations) of public functions]==============*/

/**
 * @brief   Muestra mensaje de tiempo en primera fila LCD.
 *
 * @param	time	String de mensaje a mostrar.
 * @param   size	Longitud del string.
 *
 * @note    Muestra nn:nn en pantalla.
 */
void LCD_time_msg(uint8_t *time, uint8_t size);

/**
 * @brief   Muestra mensaje en segunda fila LCD.
 *
 * @param	text	String de mensaje a mostrar.
 * @param   size	Longitud del string.
 *
 * @note    Muestra mensaje de estado en pantalla.
 */
void LCD_state_msg(uint8_t *text, uint8_t size);

/**
 * @brief   ?
 *
 * @note    ?
 */
void LCD_loading();

/**
 * @brief   Inicializa LCD.
 *
 * @note    Debe llamarse antes de utilizar el LCD.
 */
void LCD_Setup_Begin(void);

/**
 * @brief   Alterna el display en encendido y apagado.
 *
 */
void LCD_flash(void);

/*=====[Avoid multiple inclusion - end]=============================*/
#endif /* _LCD_MICROWAVE_H_ */