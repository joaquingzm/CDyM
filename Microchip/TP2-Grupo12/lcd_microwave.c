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