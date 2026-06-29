#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "monitor.h"

int main(void)
{
	monitor_init();
	sei();

	while(1)
	{
		monitor_dispatch();
	}
}

