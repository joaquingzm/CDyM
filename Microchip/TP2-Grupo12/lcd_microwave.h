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


/*=====[Definition macros of public constants]======================*/

/*=====[Prototypes (declarations) of public functions]==============*/


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
void LCD_Setup_Reposo(void);

/**
 * @brief   Inicializa LCD.
 *
 * @note    Debe llamarse antes de utilizar el LCD.
 */
void LCD_Setup_Begin(void);

/*=====[Avoid multiple inclusion - end]=============================*/
#endif /* _LCD_MICROWAVE_H_ */