#ifndef _KEYPAD4X4_H_
#define _KEYPAD4X4_H_

void keypad_init(void);
void keypad_read(char *key);

/* solo para testing */
void keypad_simulate_key(char new_key);

#endif