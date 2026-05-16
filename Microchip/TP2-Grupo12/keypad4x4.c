#include "keypad4x4.h"

static char simulated_key = 0;

void keypad_init(void)
{
	simulated_key = 0;
}

void keypad_simulate_key(char new_key)
{
	simulated_key = new_key;
}

void keypad_read(char *key)
{
	*key = simulated_key;

	/* consumir tecla */
	simulated_key = 0;
}