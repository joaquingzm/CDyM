/*
 * timer1_pwm.h
 */ 


#ifndef INCFILE2_H_
#define INCFILE2_H_

#include <stdint.h>

void timer1_init();

void timer1_pwm_set_duty_A(uint8_t duty);

void timer1_pwm_set_duty_B(uint8_t duty);

#endif /* INCFILE2_H_ */