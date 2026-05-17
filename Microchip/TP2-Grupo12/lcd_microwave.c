
/*=====[Inclusion of own header]====================================*/
#include "lcd_microwave.h"

/*=====[Inclusions of private function dependencies]================*/
#include "lcd.h"

/*=====[Definitions of private global variables]====================*/
static uint8_t display_visible=1;
static uint8_t loading[] = {' ','.','o','O','o','.'};
static uint8_t i =0;

/*=====[Prototypes (declarations) of private functions]=============*/


/*=====[Implementations of public functions]========================*/
void LCD_time_msg(uint8_t *time, uint8_t size){
	LCDGotoXY(0,0);
	LCDstring(time,size-1);
	LCDGotoXY(0,0);
}

void LCD_state_msg(uint8_t *text, uint8_t size){
	LCDclr();
	LCDGotoXY(0,1);
	LCDstring(text,size-1);
	LCDGotoXY(0,0);
}

void LCD_loading(void){
	LCDGotoXY(15,1);
	LCDsendChar(loading[i]);
	i++;
	if(i==6)i=0;
}


void LCD_Setup_Begin(){
	LCD_Init();
}

void LCD_flash(){
	if (display_visible) {
		LCDblank();
		display_visible=0;
		}else{
		LCDvisible();
		display_visible=1;
	}
}

