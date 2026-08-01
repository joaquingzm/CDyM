/*
 * ldr.h
 */ 

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef LDR_H_
#define LDR_H_

/*====[Inclusion of public function dependencies]===============*/
#include <stdint.h>

/*====[Prototypes (declarations) of public functions]============*/
void ldr_init();

uint16_t ldr_read();

/*====[Avoid multiple inclusion - end]==========================*/
#endif /* LDR_H_ */