#include "main.h"
#include "keypad4x4.h"
#include "microwave.h"
#include "lcd_microwave.h"
#include <util/delay.h>
#include "timer.h"

int main(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/*
=======
>>>>>>> ab983e6 (Ultimo commit de la night)
	timer_init();
	KEYPAD_init();
	LCD_Setup_Begin();
    microwave_init();
    while (1) 
    {
		if(FLAG_TIMER){
			microwave_update();
			FLAG_TIMER = 0;
		}
<<<<<<< HEAD
    }
	*/
	 KEYPAD_init();
	 LCD_Setup_Begin();
	 microwave_init();
<<<<<<< HEAD
=======
    /* Replace with your application code */
    while (1){
	
	}
}
>>>>>>> 5b473a9 (Antes de mergear keyboard con main pruebo en mi rama con proteus)

=======
/*
>>>>>>> 07c37fc (Código funcional en proteus sin timer con algunos retoques aun por hacer)
	 keypad_simulate_key('1');
	 microwave_update();

	 keypad_simulate_key('3');
	 microwave_update();

	 keypad_simulate_key('0');
	 microwave_update();
	 
	 keypad_simulate_key('0');
	 microwave_update();

	 keypad_simulate_key('A');
	 microwave_update();
*/

	 while(1) {
=======
		
	/*	
>>>>>>> ab983e6 (Ultimo commit de la night)
		microwave_update();
		_delay_ms(5);
		*/
    }
}
