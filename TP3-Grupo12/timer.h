/**
 * @file    timer.h
 * @brief   -
 */
/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _TIMER_
#define _TIMER_

#include <stdint.h>

/*====[Definitions of public data types]=========================*/
volatile uint8_t FLAG_TIMER;

/*====[Prototypes (declarations) of public functions]============*/
void timer_init(void);

/*====[Avvoid multiple inclusion - end]==========================*/
#endif /* _TIMER_ */