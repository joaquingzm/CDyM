#include "main.h"
#include "keypad4x4.h"
#include "microwave.h"
#include "lcd_microwave.h"
//#include "timer.h"

int main(void)
{
<<<<<<< HEAD
	/*
	timer_init();
    microwave_init();
    while (1) 
    {
		if(FLAG_TIMER){
			microwave_update();
			FLAG_TIMER = 0;
		}
    }
	*/
	 keypad_init();
	 lcd_init();
	 microwave_init();
=======
    /* Replace with your application code */
    while (1){
	
	}
}
>>>>>>> 5b473a9 (Antes de mergear keyboard con main pruebo en mi rama con proteus)

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
}
