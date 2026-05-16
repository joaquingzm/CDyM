#ifndef _LCD_H_
#define _LCD_H_

void lcd_init(void);
void lcd_clear(void);
void lcd_write_string(const char *str);

/* testing */
const char* lcd_get_content(void);

#endif