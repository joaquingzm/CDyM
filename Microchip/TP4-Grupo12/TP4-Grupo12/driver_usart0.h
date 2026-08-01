/*
 * driver_usart0.h
 */

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _DRIVER_USART0_
#define _DRIVER_USART0_

/*====[Inclusion of public function dependencies]===============*/
#include <stdbool.h>
#include <stdint.h>

/*====[Definitions of public constants]==========================*/
#define USART0_UBRR(baud, f_cpu)   ((f_cpu) / (16UL * (baud)) - 1UL)

/*====[Prototypes (declarations) of public functions]============*/
void usart0_init(uint32_t baud_rate, uint32_t f_cpu);

bool usart0_rx_overflow(void);

bool usart0_tx_overflow(void);

bool usart0_rx_event(void);
 
bool usart0_rx_available(void);
 
bool usart0_tx_available(void);
 
bool usart0_read(uint8_t *c);
 
bool usart0_write(uint8_t c);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _DRIVER_USART0_ */