/**
 * @file    terminal.h
 * @brief   - 
 
 * @details
 * -
 */

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _TERMINAL_
#define _TERMINAL_

/*====[Inclusion of public function dependencies]===============*/
#include <stdbool.h>
#include <stdint.h>

/*====[Prototypes (declarations) of public functions]============*/

/**
 * @brief   -
 */
void terminal_init(uint32_t f_cpu, char *initial_msg);

/**
 * @brief   -
 */
void terminal_dispatch(void);

/**
 * @brief   -
 */
uint8_t terminal_show_msg(char *msg);

/**
 * @brief   -
 */
bool terminal_line_available(void);

/**
 * @brief   -
 */
bool terminal_get_line(char *buf, uint8_t max_len);

/**
 * @brief   -
 */
bool terminal_rx_overflow(void);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _TERMINAL_ */