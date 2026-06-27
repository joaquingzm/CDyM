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

/*====[Definitions of public data types]=========================*/
volatile uint8_t RX_FLAG;
volatile uint8_t TX_FLAG;
volatile uint8_t RX_OVF_FLAG;
volatile uint8_t TX_OVF_FLAG;

/*====[Prototypes (declarations) of public functions]============*/

/**
 * @brief   Inicializa el driver, configura flags, interrupciones
			y velocidades de transferencia de datos del periférico USART0.
			Además inicializa flags de eventos internos.
 */
void usart0_init(uint32_t baud_rate, uint32_t f_cpu);

/**
 * @brief   Permite conocer si hubo overflow del buffer de recepción.
			Una vez llamada, baja el flag de overflow interno.
 */
bool usart0_rx_overflow(void);

/**
 * @brief   Permite conocer si hubo overflow del buffer de recepción.
			Una vez llamada, baja el flag de overflow interno.
 */
bool usart0_tx_overflow(void);

/**
 * @brief   Permite conocer si hubo un evento de recepción.
			Una vez llamda, baja el flag de recepción interno.
 */
bool usart0_rx_event(void);

/**
 * @brief   Permite conocer si el buffer de recepción tiene tramas
			no leídas.
 */
bool usart0_rx_available(void);

/**
 * @brief   Permite conocer si el buffer de recepción tiene tramas
			no leídas.
 */
bool usart0_tx_available(void);

/**
 * @brief   Permite leer una trama del buffer de recepción.
 */
bool usart0_read(uint8_t *c);

/**
 * @brief   Permite encolar una trama al buffer de transmisión.
 */
bool usart0_write(uint8_t c);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _DRIVER_USART0_ */