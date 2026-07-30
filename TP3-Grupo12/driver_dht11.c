/*===== [Inclusion of own header] ===============================*/
#include "driver_dht11.h"
#define F_CPU 16000000UL
/*===== [Inclusions of private function dependencies] ==========*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*===== [Definition macros of private constants] ================*/
#define DHT_PORT        PORTC
#define DHT_DDR         DDRC
#define DHT_PINR        PINC
#define DHT_BIT         PORTC0

/*===== [Private function-like macros] ==========================*/
#define DHT_SET_OUTPUT()    (DHT_DDR |=  (1 << DHT_BIT))
#define DHT_SET_INPUT() (DHT_DDR &= ~(1 << DHT_BIT))
#define DHT_LOW()           (DHT_PORT &= ~(1 << DHT_BIT))
#define DHT_HIGH()          (DHT_PORT |=  (1 << DHT_BIT))
#define DHT_READ()          (DHT_PINR &   (1 << DHT_BIT)) 

/*===== [Definitions of private global variables] ===============*/
static uint8_t dht_raw[5]; //40bit de respuesta del sensor

/*===== [Prototypes (declarations) of private functions] =======*/
static dht11_status_t dht_check_presence(void);
static dht11_status_t dht_read_raw(void);


/*===== [Implementations of public functions] ==================*/
dht11_status_t dht11_read(dht11_data_t *data)
{
	dht11_status_t status;
	cli();
	status = dht_check_presence();	//Verificamos el estado del sensor
	if (status != DHT11_OK){
		sei();	
		return status;	
	}
	
	status = dht_read_raw();
	if (status != DHT11_OK){
		sei();
		return status;
	}

	/* checksum */ //Verifica que la suma de los datos enviados mod 256 sea igual al que tiene el sensor
	if ((uint8_t)(dht_raw[0] + dht_raw[1] + dht_raw[2] + dht_raw[3]) != dht_raw[4]){
		sei();
		return DHT11_ERR_CHECKSUM;	
	}

	data->humidity_int    = dht_raw[0];
	data->humidity_dec    = dht_raw[1];
	data->temperature_int = dht_raw[2];
	data->temperature_dec = dht_raw[3];

	sei();
	return DHT11_OK;
}

static dht11_status_t dht_wait_level(uint8_t level, uint16_t timeout_us)
{
	while (timeout_us--) {
		if (DHT_READ() == level) {
			return DHT11_OK;
		}
		_delay_us(1);
	}

	return DHT11_ERR_TIMEOUT;
}

static dht11_status_t dht_check_presence(void)
{

	//START
	DHT_SET_OUTPUT();	//Enviamos 18ms de low 
	DHT_LOW();
	_delay_ms(18);
	DHT_SET_INPUT();	//Volvemos a setear pin como input
	_delay_us(30);

	 //Sensor response 
	if (dht_wait_level(0, 60) != DHT11_OK) {
		return DHT11_ERR_NO_RESPONSE;
	}

	if (dht_wait_level(1, 160) != DHT11_OK) {
		return DHT11_ERR_NO_RESPONSE;
	}

	if (dht_wait_level(0, 160) != DHT11_OK) {
		return DHT11_ERR_NO_RESPONSE;
	}
	return DHT11_OK;
	
}

static dht11_status_t dht_read_raw(void)
{
	uint8_t i, j;

	for (i = 0; i < 5; i++) {
		dht_raw[i] = 0;
	}

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 8; j++) {

			// Esperar el comienzo del HIGH
			if (dht_wait_level(1, 160) != DHT11_OK)
			return DHT11_ERR_TIMEOUT;

			// Medir cuánto dura el HIGH
			uint8_t pulse_width = 0;
			while (DHT_READ()) {
				_delay_us(1);
				pulse_width++;

				if (pulse_width >= 120)
				return DHT11_ERR_TIMEOUT;
			}

			// Si duró más de ~40 us, es un 1
			if (pulse_width >= 40) {
				dht_raw[i] |= (1 << (7 - j));
			}
		}
	}

	return DHT11_OK;
}