<<<<<<< HEAD
#include "lcd_microwave.h"
#include <stdio.h>
#include <string.h>

static char lcd_buffer[64];

void lcd_init(void)
{
	lcd_buffer[0] = '\0';
}

void lcd_clear(void)
{
	lcd_buffer[0] = '\0';

	printf("[LCD CLEAR]\n");
}

void lcd_write_string(const char *str)
{
	strncpy(lcd_buffer, str, sizeof(lcd_buffer));

	printf("[LCD]: %s\n", lcd_buffer);
}

const char* lcd_get_content(void)
{
	return lcd_buffer;
}
=======
/*=====[Inclusion of own header]====================================*/
#include "lcd_microwave.h"

/*=====[Inclusions of private function dependencies]================*/
#include "lcd.h"

/*=====[Definition macros of private constants]=====================*/


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


/*=====[Prototypes (declarations) of private functions]=============*/


/*=====[Implementations of public functions]========================*/
void LCD_Setup_Reposo(void){
	LCDclr();
	LCDGotoXY(0,0);
	LCDescribeDato(00,2);
	LCDsendChar(':');
	LCDescribeDato(00,2);
}

void LCD_Reposo_Refresh(uint8_t num){
	static uint8_t i=4;
	
	LCDGotoXY(i,0);
	LCDescribeDato(num,1);

	if (i==0){
		i=4;
	}else {
		i--;
		if (i==2) i--;	
	}
}

void LCD_Setup_Begin(){
	LCD_Init();
}

/*=====[Implementations of interrupt functions]=====================*/
//void UART0_IRQHandler(void) {
// ...
//}

/*=====[Implementations of private functions]=======================*/
>>>>>>> 21ddb75 (LCD header y source)
