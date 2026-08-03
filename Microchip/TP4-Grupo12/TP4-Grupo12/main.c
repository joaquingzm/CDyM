/*
 * TP4-Grupo12.c
 */ 

#define F_CPU 16000000UL
#include "sistema.h"
#include "ledRGB.h"
#include <stdint.h>
#include <avr/interrupt.h>

int main(void)
{
	sistema_init();
	sei();
    while (1) 
    {
		sistema_dispatch();
    }
}

