/*
 * ledRGB.h
 */ 

#ifndef LEDRG_H_
#define LEDRG_H_

#include <stdint.h>

void ledRGB_init();

void ledRGB_set(uint8_t r, uint8_t g, uint8_t b);

#endif /* LEDRGB_H_ */