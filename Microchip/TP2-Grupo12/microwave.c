/*====[Inclusion of own header]==================================*/
#include "microwave.h"

/*====[Inclusion of private function dependencies]===============*/
#include "keypad4x4.h"
#include "lcd_microwave.h"
#include "led.h"
#include <stdint.h>

/*====[Definitions of private global variables]==================*/
static microwave_state_t state;
static uint8_t cooking_time_m;
static uint8_t cooking_time_s;
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

static void microwave_show_time(void);
static void microwave_add_digit(uint8_t digit);
static void microwave_decrease_cooking_time();
static void microwave_increase_cooking_time(uint8_t secs_to_inc);
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
	cooking_time_m = 0u;
	cooking_time_s = 0u;
	blink_count = 0u;
	state_call_count = 0u;
	blink_flag = 0u;
	door_open = 0u;
	new_state = 1u;
	key = 0;

	lcd_clear();
	microwave_show_time();
}

void microwave_update(){
	key = 0;
	keypad_read(&key);

	if (key == 'D')
	{
		door_open = (uint8_t)!door_open;
	}

	microwave[state]();
}

/*====[Implementations of private functions]=====================*/
static void microwave_state_idle(void)
{
	if (new_state)
	{
		lcd_clear();
		microwave_show_time();
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
		microwave_increase_cooking_time(30u);
		microwave_show_time();
	}
	else if ((key == 'A') && (cooking_time_s > 0u || cooking_time_m > 0u) && (door_open == 0u))
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
		lcd_clear();
		microwave_show_time();
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
		microwave_increase_cooking_time(30u);
		microwave_show_time();
	}
	/* Cada 10 llamadas = 1 segundo, si update corre cada 100 ms */
	state_call_count++;
	if (state_call_count >= 10u)
	{
		state_call_count = 0u;

		if (cooking_time_s > 0u || cooking_time_m > 0u)
		{
			microwave_decrease_cooking_time();
			microwave_show_time();
		}

		if (cooking_time_s == 0u && cooking_time_m == 0u)
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
		lcd_clear();
		lcd_write_string("PAUSED");
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
	if ((key == 'A') && (cooking_time_m > 0u || cooking_time_s > 0u) && (door_open == 0u))
	{
		state = COOKING;
		new_state = 1u;
		state_call_count = 0u;
		return;
	}
	if (key == 'C')
	{
		microwave_increase_cooking_time(30u);
		microwave_show_time();
	}
}

static void microwave_state_finished(void)
{
	if (new_state)
	{
		lcd_clear();
		lcd_write_string("** FINISHED **");

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

		lcd_clear();
		led_alarm_off();
		if (blink_flag)
		{
			led_alarm_on();
			lcd_write_string("** FINISHED **");
		}
	}
	/* 5 segundos totales -> 50 llamadas de 100 ms */
	state_call_count++;
	if (state_call_count >= 50u)
	{
		cooking_time_s = 0u;
		cooking_time_m = 0u;
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
	cooking_time_s = 0u;
	cooking_time_m = 0u;
	state_call_count = 0u;
	blink_count = 0u;
	blink_flag = 0u;
	microwave_show_time();
}

static void microwave_add_digit(uint8_t digit)
{
	if (digit > 9u)
	{
		return;
	}
	cooking_time_m = (uint8_t)((cooking_time_m*10u)%100u + cooking_time_s/10u);
	cooking_time_s = (uint8_t)((cooking_time_s*10u)%100u + digit);
	microwave_show_time();
}

static void microwave_decrease_cooking_time()
{
	if (cooking_time_s > 0u)
	{
		cooking_time_s--;
	}
	else if (cooking_time_m > 0u)
	{
		cooking_time_m--;
		cooking_time_s = 59;
	}
	else
	{
		// ya en 00:00
	}
}

static void microwave_increase_cooking_time(uint8_t secs_to_inc)
{
	cooking_time_s = cooking_time_s + secs_to_inc;
	if (cooking_time_s > 59u)
	{
		if (cooking_time_m < 99u)
		{
			cooking_time_s = cooking_time_s % 60;
			cooking_time_m++;
		}
		else 
		{
			cooking_time_s = 59u;
		}
	}
}

static void microwave_show_time(void)
{
	char buffer[6];

	buffer[0] = (char)('0' + ((cooking_time_m/ 10u) % 10u));
	buffer[1] = (char)('0' + (cooking_time_m % 10u));
	buffer[2] = ':';
	buffer[3] = (char)('0' + (cooking_time_s / 10u));
	buffer[4] = (char)('0' + (cooking_time_s % 10u));
	buffer[5] = '\0';

	lcd_clear();
	lcd_write_string(buffer);
}