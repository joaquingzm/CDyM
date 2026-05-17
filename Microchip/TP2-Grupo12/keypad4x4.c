/*=====[Inclusion of own header]====================================*/
#include "keypad4x4.h"

/*=====[Inclusions of private function dependencies]================*/
#include <util/delay.h>

/*=====[Definition macros of private constants]=====================*/
#define KEYPAD_COL_MASK     0x3C    // Mascara de columnas en PORTD
#define KEYPAD_ROW_MASK     0x99    // Mascara de filas en PORTB

/*=====[Private function-like macros]===============================*/
//#define moduleConfig moduleInit

/*=====[Definitions of private data types]=========================*/
// Function pointer data type
//typedef void (*FuncPtrPrivado_t)(void *);

/*=====[Definitions of external public global variables]============*/
//extern int32_t varGlobalExterna;

/*=====[Definitions of public global variables]=====================*/
//int32_t varGlobalPublica = 0;

/*=====[Definitions of private global variables]====================*/

static uint8_t KEYPAD_filas[4] = {0x10,0x08,0x01,0x80}; //PB4=0x10 - PB3=0x08 - PB0=0x01 - PB7=0x80	//PORTB
static uint8_t KEYPAD_columnas[4] = {0x08, 0x20, 0x10, 0x04}; // PD3=0x08, PD5=0x20, PD4=0x10, PD2=0x04 //PORTD
	
static const char KEYPAD_map[4][4] = {
	{'7','8','9','A'},
	{'4','5','6','B',},
	{'1','2','3','C',},
	{KEYPAD_NO_KEY,'0',KEYPAD_NO_KEY,'D'}
};

/*=====[Prototypes (declarations) of private functions]=============*/
//static void funPrivada(void);
static uint8_t KEYPAD_actualizar(void);


/*=====[Implementations of public functions]========================*/
void KEYPAD_init(){
	//PORTB para entradas, columnas
	//PORTD para salida, filas
	
	DDRB &= ~KEYPAD_ROW_MASK;		//Pines B como entrada
	PORTB |= KEYPAD_ROW_MASK;			//1001 1001 - Pull Up de pines B
	
	DDRD |= KEYPAD_COL_MASK;			//Pines D como salida
	PORTD |= KEYPAD_COL_MASK;			//00111100	- Salidas en HIGH
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
	if(Key==Old_key) { //2da verificación
		if(Key!=Last_valid_key){ //evita múltiple detección
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificación
	return 0;
}

/*=====[Implementations of interrupt functions]=====================*/
//void UART0_IRQHandler(void) {
	// ...
//}

/*=====[Implementations of private functions]=======================*/
static uint8_t KEYPAD_actualizar(){ // Devuelve la posición del boton desde 0-15
	 uint8_t r,c;

	for(c=0; c<4; c++){
		// Pone la fila c en LOW, las demás en HIGH
		PORTD = (PORTD | KEYPAD_COL_MASK) & ~KEYPAD_columnas[c];
		_delay_us(1);

		for(r=0; r<4; r++){
			if(!(PINB & KEYPAD_filas[r])){
				return KEYPAD_map[r][c];
			}
		}
	}
	
	return KEYPAD_NO_KEY;
}