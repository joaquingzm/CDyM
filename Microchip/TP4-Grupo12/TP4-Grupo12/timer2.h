/*
 * timer2.h
 */ 

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef TIMER2_H_
#define TIMER2_H_

#include <stdint.h>

/*====[Definitions of public data types]=========================*/
volatile uint8_t FLAG_TIMER;

/*====[Prototypes (declarations) of public functions]============*/
void timer2_init(void);

/*====[Avoid multiple inclusion - end]==========================*/
#endif /* TIMER2_H_ */