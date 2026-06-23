

static char buffer[32];
static uint8_t idx = 0;

void terminal_init();

void terminal_rx_store_char(char c)
{
	buffer[idx++] = c;
	if (c == 'n')
	{
		buffer[idx] = '0';
		idx = 0;
	}
}

void terminal_task();

void terminal_println(const char *s);
