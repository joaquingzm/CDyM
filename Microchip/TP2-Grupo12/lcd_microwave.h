
/***********************************************************************
* Archivo: lcd_microwave.h
* Descripci?n:
*   Implementaci?n del LCD del horno microondas.
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