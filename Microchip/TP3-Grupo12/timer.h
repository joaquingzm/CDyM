/**
 * @file    timer.h
 * @brief   
 *
 * @details
 * Este módulo provee las funciones necesarias para inicializar
 * y controlar el timer utilizado como base temporal del sistema.
 */
/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _TIMER_
#define _TIMER_

#include <stdint.h>
/*====[Definitions of public data types]=========================*/
volatile uint8_t FLAG_TIMER;

/*====[Prototypes (declarations) of public functions]============*/
/***********************************************************************
* Propósito de la función:
*	
*
* Parámetros de entrada (tipo, rango y formato) :
*	Ninguno.
*
* Parámetros de salida (tipo, rango y formato) :
*	Ninguno.
*
* Otros comentarios:
*	Debe ejecutarse una única vez al inicio del sistema
***********************************************************************/
void timer_init(void);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _TIMER_ */