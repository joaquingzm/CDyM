/**
 * @file    driver_usart0.h
 * @brief   Driver para periférico USART0 del ATmega328P configurado en modo
 *			comunicación serie asíncrona, con transmisión y recepción full-duplex
 *			mediante interrupciones y buffers circulares.
 *
 * @details
 * -
 */

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _DRIVER_USART0_
#define _DRIVER_USART0_

/*====[Inclusion of public function dependencies]===============*/
#include <stdbool.h>
#include <stdint.h>

/*====[Definitions of public constants]==========================*/
/**
 * @brief Calcula el valor UBRR para una frecuencia de CPU y baud rate dados.
 *        Asume modo normal (U2X0 = 0, oversampling 16x).
 *
 * Ejemplo: usart0_init(9600, F_CPU)
 *          internamente usa USART0_UBRR(9600, 16000000) = 103
 */

#define USART0_UBRR(baud, f_cpu)   ((f_cpu) / (16UL * (baud)) - 1UL)

/*====[Prototypes (declarations) of public functions]============*/

/**
 * @brief   Inicializa el periferico USART0 en modo asincrono 8N1.
 *          Configura baud rate, formato de trama e interrupciones.
 *          Resetea los buffers circulares y todos los flags internos.
 *          Debe llamarse antes de sei().
 *
 * @param   baud_rate   Velocidad de transferencia en bps (ej: 9600)
 * @param   f_cpu       Frecuencia del CPU en Hz (ej: 16000000UL)
 */
void usart0_init(uint32_t baud_rate, uint32_t f_cpu);

/**
 * @brief   Indica si hubo overflow del buffer de recepcion.
 *          Ocurre cuando llega un byte y el buffer RX esta lleno.
 *          Una vez llamada, baja el flag interno.
 */
bool usart0_rx_overflow(void);

/**
 * @brief   Indica si hubo overflow del buffer de transmision.
 *          Ocurre cuando se intenta encolar un byte y el buffer TX esta lleno.
 *          Una vez llamada, baja el flag interno.
 */
bool usart0_tx_overflow(void);

/**
 * @brief   Indica si se recibio al menos un byte desde la ultima llamada.
 *          Util para despertar logica que espera cualquier dato entrante.
 *          Una vez llamada, baja el flag interno.
 */
bool usart0_rx_event(void);
 
/**
 * @brief   Indica si el buffer de recepcion tiene al menos un byte sin leer.
 *          No modifica ningun flag interno.
 */
bool usart0_rx_available(void);
 
/**
 * @brief   Indica si el buffer de transmision tiene al menos un slot libre
 *          para encolar un byte. No modifica ningun flag interno.
 */
bool usart0_tx_available(void);
 
/**
 * @brief   Lee y extrae un byte del buffer de recepcion.
 *
 * @param   c   Puntero donde se almacena el byte leido.
 * @return  true si habia un byte disponible y fue leido,
 *          false si el buffer estaba vacio.
 */
bool usart0_read(uint8_t *c);
 
/**
 * @brief   Encola un byte en el buffer de transmision.
 *          La ISR USART_UDRE lo enviara por hardware automaticamente.
 *
 * @param   c   Byte a transmitir.
 * @return  true si el byte fue encolado correctamente,
 *          false si el buffer estaba lleno.
 */
bool usart0_write(uint8_t c);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _DRIVER_USART0_ */