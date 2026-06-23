

static volatile void *(rx_callback)(char) = 0;

void usart_register_rx_callback(void (*cb)(char))
{
	rx_callback = cb;
}

void usart_putchar(char c);

ISR(USART_RX_vect)
{
	char c = UDR0;
	if(c = '\n') flag_salto_linea = 1;
	if(rx_callback) rx_callback(c);
}