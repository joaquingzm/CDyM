/***********************************************************************
* Archivo: microwave.h
* Descripción:
*   Implementación de la MEF del horno microondas.
*
***********************************************************************/

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _MICROWAVE_
#define _MICROWAVE_

/*====[Definitions of public data types]=========================*/
typedef enum {IDLE, COOKING, FINISHED, PAUSED} microwave_state_t;
	
/*====[Prototypes (declarations) of public functions]============*/
/***********************************************************************
* Propósito de la función: 
*	Inicializa  el modulo del horno de microondas.
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
void microwave_init(void);

/***********************************************************************
* Propósito de la función:
*	Indica al modulo del horno de microondas que lea las entradas
*	y correspondiente a su estado actual, cambie o no de estado.
*
* Parámetros de entrada (tipo, rango y formato) :
*	Ninguno.
*
* Parámetros de salida (tipo, rango y formato) :
*	Ninguno.
*
* Otros comentarios:
*	Debe ejecutarse periodicamente a 100ms
***********************************************************************/
void microwave_update(void);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _MICROWAVE */