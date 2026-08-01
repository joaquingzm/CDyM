/*
 * TP4-Grupo12.c
 */ 

#define F_CPU 16000000UL
#include "sistema.h"
#include <avr/interrupt.h>

int main(void)
{
	sei();
	sistema_init();
    while (1) 
    {
		sistema_dispatch();
    }
}

