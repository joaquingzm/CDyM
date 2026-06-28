#define F_CPU 16000000UL

#include <avr/io.h>
#include "driver_usart0.h"
#include "terminal.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
//añado pruebas del sensor
#include "driver_dht11.h"
#include "monitor.h"

int main(void)
{
	monitor_init();
	timer_init();
	sei();

	while(1)
	{
		monitor_dispatch();
	}
}

