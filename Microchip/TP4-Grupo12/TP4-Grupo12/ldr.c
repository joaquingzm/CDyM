/*
 * ldr.c
 */ 

/*====[Inclusion of own header]==================================*/
#include "ldr.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>

/*====[Definitions of private global variables]==================*/
#define VREF 5

/*====[Implemenations of public functions]=======================*/
void ldr_init()
{
	// Elijo como referencia Avcc (5V), dejo Right Adjust Result, elijo PC3 como entrada
	//    | Vref es AVcc|   Input Chann ADC3  |
	ADMUX |= (1<<REFS0) | (1<<MUX1) | (1<<MUX0);
	
	// Configuro ADC
	//     | ADC Enable | Prescaler 128 para input clock 125kHz < 200kHz 
	//     |            | para que funcione de manera óptima
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

uint16_t ldr_read()
{
	// Inicio Conversion
	ADCSRA |= (1<<ADSC);
	
	// Espero a que termine la conversión (25 ciclos -> (1/125kHz)*25=200us)
	while((ADCSRA&(1<<ADIF))==0);
	
	// Borro flag
	ADCSRA |= (1<<ADIF);
	
	// Retorno valor convertido
	return (uint16_t) ADC;
}