/*
 * timer1_pwm.h
 */ 

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef TIMER1_PWM_H_
#define TIMER1_PWM_H_

/*====[Inclusion of public function dependencies]================*/
#include <stdint.h>

/*====[Prototypes (declarations) of public functions]============*/
void timer1_init();

void timer1_pwm_set_duty_A(uint8_t duty);

void timer1_pwm_set_duty_B(uint8_t duty);

/*====[Avoid multiple inclusion - end]==========================*/
#endif /* TIMER1_PWM_H_ */