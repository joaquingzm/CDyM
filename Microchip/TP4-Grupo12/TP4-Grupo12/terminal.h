/**
 * @file    terminal.h
 */

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _TERMINAL_
#define _TERMINAL_

/*====[Inclusion of public function dependencies]===============*/
#include <stdbool.h>
#include <stdint.h>

/*====[Prototypes (declarations) of public functions]============*/
void terminal_init(uint32_t f_cpu, char *initial_msg);

void terminal_dispatch(void);

uint8_t terminal_show_msg(char *msg);

bool terminal_line_available(void);

bool terminal_get_line(char *buf, uint8_t max_len);

bool terminal_rx_overflow(void);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _TERMINAL_ */