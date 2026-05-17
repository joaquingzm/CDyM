/*====[Inclusion of own header]==================================*/
#include "microwave.h"
 
/*====[Inclusion of private function dependencies]===============*/
#include "keypad4x4.h"
#include "lcd_microwave.h"
#include "led.h"
#include <stdint.h>

/*====[Definitions of private global variables]==================*/
static microwave_state_t state;
static uint16_t cooking_time;
static uint16_t typed_mmss;
static uint16_t blink_count;
static uint16_t state_call_count;
static uint8_t blink_flag;
static uint8_t door_open;
static uint8_t new_state;
static char key;

/*====[Prototypes (declarations) of private functions]===========*/
static void microwave_state_idle(void);
static void microwave_state_cooking(void);
static void microwave_state_paused(void);
static void microwave_state_finished(void);

static void microwave_show_time(uint16_t seconds);
static void microwave_add_digit(uint8_t digit);
static void microwave_clear_time(void);
static uint16_t microwave_seconds_to_mmss(uint16_t seconds);
static uint16_t microwave_mmss_to_seconds(uint16_t mmss);

static void (*microwave[])(void)=
{
	microwave_state_idle,
	microwave_state_cooking,
	microwave_state_finished,
	microwave_state_paused,
};

/*====[Implemenations of public functions]=======================*/
void microwave_init(){
	state = IDLE;
	cooking_time = 0u;
	typed_mmss = 0u;
	blink_count = 0u;
	state_call_count = 0u;
	blink_flag = 0u;
	door_open = 0u;
	new_state = 1u;
	key = 0;

	//LCDclr();
	led_init();
	LCD_Setup_Begin();
	microwave_show_time(0u);
}

void microwave_update(){
	key = 0;
	uint8_t hay_tecla = KEYPAD_scan(&key);

	if (hay_tecla){
		if (key == 'D'){
			door_open = (uint8_t)!door_open;
		}	
	}
	

	microwave[state]();
}

/*====[Implementations of private functions]=====================*/
static void microwave_state_idle(void)
{
	if (new_state)
	{
		led_alarm_off();
		LCDclr(); // Esto se hace solo una vez en el modo idle? o siempre? podría implementar cartel "Introduzca ..."
		microwave_show_time(cooking_time);
		new_state = 0u;
	}

	if (key >= '0' && key <= '9')
	{
		microwave_add_digit((uint8_t)(key - '0'));
	}
	else if (key == 'B')
	{
		microwave_clear_time();
	}
	else if (key == 'C')
	{
		cooking_time += 30u;
		typed_mmss = microwave_seconds_to_mmss(cooking_time);
		microwave_show_time(cooking_time);
	}
	else if ((key == 'A') && (cooking_time > 0u) && (door_open == 0u))
	{
		state = COOKING;
		new_state = 1u;
		state_call_count = 0u;
		blink_count = 0u;
		blink_flag = 0u;
	}
}

static void microwave_state_cooking(void)
{
	if (new_state)
	{
		LCDclr();
		microwave_show_time(cooking_time);
		new_state = 0u;
		state_call_count = 0u;
		blink_count = 0u;
		blink_flag = 0u;

		led_magnetron_on();
		led_light_on();
	}
	if (door_open || (key == 'B'))
	{
		state = PAUSED;
		new_state = 1u;
		state_call_count = 0u;
		return;
	}
	if (key == 'C')
	{
		cooking_time += 30u;
		typed_mmss = microwave_seconds_to_mmss(cooking_time);
		microwave_show_time(cooking_time);
	}
	/* Cada 10 llamadas = 1 segundo, si update corre cada 100 ms */
	state_call_count++;
	if (state_call_count >= 10u)
	{
		state_call_count = 0u;

		if (cooking_time > 0u)
		{
			cooking_time--;
			typed_mmss = microwave_seconds_to_mmss(cooking_time);
			microwave_show_time(cooking_time);
		}

		if (cooking_time == 0u)
		{
			state = FINISHED;
			new_state = 1u;
			state_call_count = 0u;
			blink_count = 0u;
			blink_flag = 0u;
			return;
		}
	}
}

static void microwave_state_paused(void)
{
	if (new_state)
	{
		LCDclr();
		LCDstring("PAUSED",6);
		new_state = 0u;
		state_call_count = 0u;

		led_magnetron_off();
		led_light_off();
	}
	if (key == 'B')
	{
		microwave_clear_time();
		state = IDLE;
		new_state = 1u;
		return;
	}
	if ((key == 'A') && (cooking_time > 0u) && (door_open == 0u))
	{
		state = COOKING;
		new_state = 1u;
		state_call_count = 0u;
		return;
	}
	if (key == 'C')
	{
		cooking_time += 30u;
		typed_mmss = microwave_seconds_to_mmss(cooking_time);
		microwave_show_time(cooking_time);
	}
}

static void microwave_state_finished(void)
{
	if (new_state)
	{
		LCDclr();
		LCDstring("** FINISHED **",14);
		//LCD_finished();

		new_state = 0u;
		state_call_count = 0u;
		blink_count = 0u;
		blink_flag = 1u;

		led_magnetron_off();
		led_light_off();
	}
	/* Parpadeo cada 500 ms -> 5 llamadas de 100 ms */
	blink_count++;
	if (blink_count >= 5u)
	{
		blink_count = 0u;
		blink_flag = (uint8_t)!blink_flag;

		LCDclr();
		//LCD_flash();
		led_alarm_off();
		if (blink_flag)
		{
			led_alarm_on();
			LCDstring("** FINISHED **",14);
			//LCD_flash();
		}
	}
	/* 5 segundos totales -> 50 llamadas de 100 ms */
	state_call_count++;
	if (state_call_count >= 50u)
	{
		cooking_time = 0u;
		typed_mmss = 0u;
		state = IDLE;
		new_state = 1u;
		state_call_count = 0u;
		blink_count = 0u;
		blink_flag = 0u;
		return;
	}
}

static void microwave_clear_time(void)
{
	cooking_time = 0u;
	typed_mmss = 0u;
	state_call_count = 0u;
	blink_count = 0u;
	blink_flag = 0u;
	microwave_show_time(0u);
}

static void microwave_add_digit(uint8_t digit)
{
	if (digit > 9u)
	{
		return;
	}
	/* desplaza el número MMSS e incorpora el nuevo dígito */
	typed_mmss = (uint16_t)((typed_mmss % 1000u) * 10u + digit);
	cooking_time = microwave_mmss_to_seconds(typed_mmss);
	microwave_show_time(cooking_time);
}

static uint16_t microwave_mmss_to_seconds(uint16_t mmss)
{
	uint16_t minutes = mmss / 100u;
	uint16_t seconds = mmss % 100u;
	return (uint16_t)(minutes * 60u + seconds);
}

static uint16_t microwave_seconds_to_mmss(uint16_t seconds)
{
	uint16_t minutes = seconds / 60u;
	uint16_t secs = seconds % 60u;
	return (uint16_t)(minutes * 100u + secs);
}

static void microwave_show_time(uint16_t seconds)
{
	char buffer[6];
	uint16_t minutes;
	uint16_t secs;
	if (seconds > 5999u)
	{
		seconds = 5999u; /* límite 99:59 */
	}
	minutes = seconds / 60u;
	secs = seconds % 60u;

	buffer[0] = (char)('0' + ((minutes / 10u) % 10u));
	buffer[1] = (char)('0' + (minutes % 10u));
	buffer[2] = ':';
	buffer[3] = (char)('0' + (secs / 10u));
	buffer[4] = (char)('0' + (secs % 10u));
	buffer[5] = '\0';

	LCDclr();
	LCDstring(buffer, sizeof(buffer)-1);
}