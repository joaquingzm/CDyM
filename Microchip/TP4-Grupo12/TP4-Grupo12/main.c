/*
 * TP4-Grupo12.c
 */ 

#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "ledRGB.h"
#include "ldr.h"

int main(void)
{
	uint8_t cont[3] = {0,0,0};
	uint8_t i = 0;
	uint8_t ldr;
    ledRGB_init();
	ldr_init();
	ledRGB_set(0,0,0);
	sei();
	cont[i]=cont[i]+16;
    while (1) 
    {
		ledRGB_set(cont[0],cont[1],cont[2]);
		
		ldr = ldr_read();
		
		cont[i]=(uint8_t)((uint32_t)((ldr*255)/1024));
		_delay_ms(100);
    }
}

