/**
 * @file    microwave.h
 * @brief   Implementación de la lógica de un horno de microondas.
 *
 * @details
 * Provee funciones para inicializar y ejecutar
 * ciclos de funcionamiento del horno de microondas.
 * Debe llamarse cada 100ms para funcionar correctamente.
 */

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _MICROWAVE_
#define _MICROWAVE_

/*====[Definitions of public data types]=========================*/
typedef enum {IDLE, COOKING, FINISHED, PAUSED} microwave_state_t;

/*====[Prototypes (declarations) of public functions]============*/
/**
 * @brief   Inicializa variables internas del horno de microondas.
 *
 * @note    Debe llamarse antes de utilizar cualquier otra función.
 */
void microwave_init(void);


/**
 * @brief   Ejecuta la lógica del microondas, leyendo el 
 *			estado de sus entradas y cambiando su estado
 *			en consecuencia.
 */
void microwave_update(void);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _MICROWAVE */