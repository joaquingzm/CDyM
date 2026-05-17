/***********************************************************************
* Archivo: lcd_microwave.h
* Descripción:
*   Implementación del LCD del horno microondas.
*
***********************************************************************/

/*=====[Avoid multiple inclusion - begin]===========================*/
#ifndef _LCD_MICROWAVE_H_
#define _LCD_MICROWAVE_H_

/*=====[Inclusions of public function dependencies]=================*/
#include <stdint.h>

/*=====[Definition macros of public constants]======================*/

/*=====[Prototypes (declarations) of public functions]==============*/

/**
 * @brief   Muestra mensaje en segunda fila LCD.
 *
 * @param	string de mensaje y sizeof mensaje 
 *
 * @note    Muestra nn:nn en pantalla.
 *          Debe llamarse antes de iniciar el conteo.
 */
void LCD_state_msg(uint8_t *text, uint8_t size);

/**
 * @brief   Muestra mensaje en segunda fila LCD.
 *
 * @param	string de mensaje y sizeof mensaje 
 *
 * @note    Muestra nn:nn en pantalla.
 *          Debe llamarse antes de iniciar el conteo.
 */
void LCD_loading();

/**
 * @brief   Setear tiempo de cocción de microondas.
 *
 * @param	n Numero válido de la forma 2400 = 24:00
 *
 * @note    Muestra nn:nn en pantalla.
 *          Debe llamarse antes de iniciar el conteo.
 */
void LCD_Start_Setup(uint16_t time);
/**
 * @brief   Actualiza LCD solo si hubo flanco de reloj.
 *
 * @note    Cada iteración verifica el conteo
 */
void LCD_Start_Refresh(void);
/**
 * @brief   Actualiza tiempo de microondas desde segundos a minutos.
 *
 * @param	n Numero válido de 0-9 para agregar al tiempo
 *
 * @note    Muestra nn:nn en pantalla.
 *          Debe llamarse al presionarse una tecla del keypad.
 */
void LCD_Reposo_Refresh(uint8_t n);
/**
 * @brief   Inicializa LCD para el estado de reposo.
 *
 * @note    Muestra 00:00 en pantalla.
 *          Debe llamarse al pasar al estado de reposo.
 */
void LCD_Reposo_Setup(void);

/**
 * @brief   Inicializa LCD.
 *
 * @note    Debe llamarse antes de utilizar el LCD.
 */
void LCD_Setup_Begin(void);

/*=====[Avoid multiple inclusion - end]=============================*/
#endif /* _LCD_MICROWAVE_H_ */

