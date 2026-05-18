/**
 * @file    keypad4x4.h
 * @brief   Driver keypad4x4.
 *
 * @details
 * Provee funciones para inicializar y controlar
 * 
 *
 * El módulo permite:
 *
 *
 */
/*=====[Avoid multiple inclusion - begin]===========================*/
#ifndef _KEYPAD_H_
#define _KEYPAD_H_

/*=====[Inclusions of public function dependencies]=================*/
#include <avr/io.h>
#include <stdint.h>

/*=====[Definition macros of public constants]======================*/
#define KEYPAD_NO_KEY   0xFF

/*=====[Prototypes (declarations) of public functions]==============*/

/**
 * @brief   Inicializa los puertos del teclado matricial 4x4.
 *
 * @note    PORTB como entradas (filas) con pull-up activado.
 *          PORTD como salidas (columnas) en HIGH.
 *          Debe llamarse antes de cualquier otra funci?n del m?dulo.
 */
void KEYPAD_init(void);

/**
 * @brief   Escanea el teclado y detecta una nueva tecla presionada.
 *          Tiene doble verificacion y evita multiple deteccion.
 *
 * @param   pkey    Puntero donde se almacena la tecla detectada (0-15).
 *
 * @return  1       Si hay una nueva tecla presionada. El valor esta en *pkey.
 * @return  0       Si no hay nueva tecla presionada.
 */
uint8_t KEYPAD_scan(uint8_t *pkey);

/*=====[Avoid multiple inclusion - end]=============================*/
#endif /* _KEYPAD_H_ */
