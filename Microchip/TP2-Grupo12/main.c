#include "main.h"
#include "keypad4x4.h"
#include "microwave.h"
#include "lcd_microwave.h"
#include <util/delay.h>
#include "timer.h"

int main(void)
{	
	timer_init();
    microwave_init();
    while (1) 
    {
		if(FLAG_TIMER){
			microwave_update();
			FLAG_TIMER = 0;
		}
    }
}
