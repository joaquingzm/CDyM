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
void terminal_init(void);

/**
 * @brief   -
 */
void terminal_dispatch(void);

/**
 * @brief   -
 */
void terminal_send(const char *s);

/**
 * @brief   -
 */
bool terminal_line_available(void);

/**
 * @brief   -
 */
bool terminal_get_line(char *buf, uint8_t max_len);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _TERMINAL_ */