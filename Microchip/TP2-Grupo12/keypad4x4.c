/**
 * @file keypad4x4.c
 * @brief Implementación de driver de keypad4x4.
 */
/*=====[Inclusion of own header]====================================*/
#include "keypad4x4.h"
#define F_CPU 16000000L
/*=====[Inclusions of private function dependencies]================*/
#include <util/delay.h>

/*=====[Definition macros of private constants]=====================*/
#define KEYPAD_COL_MASK     0x3C    // Mascara de columnas en PORTD
// 00111100
#define KEYPAD_ROW_MASK     0x19    // Mascara de filas en PORTB
// 00011001

#define PD7_MASK 0x80
/*=====[Definitions of private global variables]====================*/
//Arreglo de filas y columnas para saber a que pines pertenece cada boton
static uint8_t KEYPAD_filas[4] = {0x10,0x08,0x01,0x80}; //PB4=0x10 - PB3=0x08 - PB0=0x01 - PD7=0x80	//
static uint8_t KEYPAD_columnas[4] = {0x08, 0x20, 0x10, 0x04}; // PD3=0x08, PD5=0x20, PD4=0x10, PD2=0x04 //

//Mapa del teclado para devolver caracter correcto seg?n bot?n presionado
static const char KEYPAD_map[4][4] = {
		{'1','2','3','A'},
		{'4','5','6','B',},
		{'7','8','9','C',},
		{KEYPAD_NO_KEY,'0',KEYPAD_NO_KEY,'D'}
};

/*=====[Prototypes (declarations) of private functions]=============*/
static uint8_t KEYPAD_actualizar(void);


/*=====[Implementations of public functions]========================*/
void KEYPAD_init(void)
{
	// Filas en PORTB
	DDRB &= ~(KEYPAD_ROW_MASK);
	PORTB |= KEYPAD_ROW_MASK;

	// PD7 como entrada
	DDRD &= ~(0x80);
	PORTD |= 0x80;

	// Columnas como salida
	DDRD |= KEYPAD_COL_MASK;
	PORTD |= KEYPAD_COL_MASK;
}					

uint8_t KEYPAD_scan (uint8_t *pkey)
{
	static uint8_t Old_key, Last_valid_key=KEYPAD_NO_KEY; // no hay tecla presionada;
	uint8_t Key;
	Key= KEYPAD_actualizar();
		
	if(Key==KEYPAD_NO_KEY){
		Old_key=KEYPAD_NO_KEY; // no hay tecla presionada
		Last_valid_key=KEYPAD_NO_KEY;
		return 0;
	}
	if(Key==Old_key) { //2da verificaci?n
		if(Key!=Last_valid_key){ //evita m?ltiple detecci?n
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificaci?n
	return 0;
}

/*=====[Implementations of private functions]=======================*/
static uint8_t KEYPAD_actualizar(void)
{
	uint8_t r, c;

	for(c = 0; c < 4; c++)
	{
		// Mantener PD7 SIEMPRE intacto + pull-up activo
		PORTD |= PD7_MASK;

		// Todas las columnas en HIGH (solo bits de columnas)
		PORTD |= KEYPAD_COL_MASK;

		// Bajar columna actual SIN tocar PD7
		PORTD &= ~(KEYPAD_columnas[c]);

		_delay_us(5);

		// Filas en PORTB
		for(r = 0; r < 3; r++)
		{
			if(!(PINB & KEYPAD_filas[r]))
			return KEYPAD_map[r][c];
		}

		// Fila PD7
		if(!(PIND & PD7_MASK))
		return KEYPAD_map[3][c];
	}

	return KEYPAD_NO_KEY;
}