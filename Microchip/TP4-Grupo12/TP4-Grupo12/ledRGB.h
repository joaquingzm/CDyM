/*
 * ledRGB.h
 */ 

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef LEDRG_H_
#define LEDRG_H_

/*====[Inclusion of public function dependencies]===============*/
#include <stdint.h>

/*====[Prototypes (declarations) of public functions]============*/
void ledRGB_init();

void ledRGB_set(uint8_t r, uint8_t g, uint8_t b);

/*====[Avoid multiple inclusion - end]==========================*/
#endif /* LEDRGB_H_ */