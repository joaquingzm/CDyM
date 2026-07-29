/*
 * TP4-Grupo12.c
 */ 

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ledRGB.h"

int main(void)
{
	uint8_t cont[3] = {0,0,0};
	uint8_t i = 0;
    ledRGB_init();
	ledRGB_set(0,0,0);
	sei();
	cont[i]=cont[i]+16;
    while (1) 
    {
		ledRGB_set(cont[0],cont[1],cont[2]);
		if(cont[i]==0)
		{
			i = (i+1) % 3;
		}
		cont[i]=cont[i]+16;
		_delay_ms(100);
    }
}

