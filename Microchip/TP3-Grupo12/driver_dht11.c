/*===== [Inclusion of own header] ===============================*/
#include "driver_dht11.h"

/*===== [Inclusions of private function dependencies] ==========*/
#include <avr/io.h>
#include <util/delay.h>

/*===== [Definition macros of private constants] ================*/
#define DHT_PORT        PORTC
#define DHT_DDR         DDRC
#define DHT_PINR        PINC
#define DHT_BIT         PC0

#define DHT_START_MS    18
#define DHT_WAIT_US     100
#define DHT_BIT_DELAY   30

/*===== [Private function-like macros] ==========================*/
#define DHT_SET_OUTPUT()    (DHT_DDR |=  (1 << DHT_BIT))
#define DHT_SET_INPUT()     (DHT_DDR &= ~(1 << DHT_BIT))
#define DHT_LOW()           (DHT_PORT &= ~(1 << DHT_BIT))
#define DHT_HIGH()          (DHT_PORT |=  (1 << DHT_BIT))
#define DHT_READ()          (DHT_PINR &   (1 << DHT_BIT))

/*===== [Definitions of private global variables] ===============*/
static uint8_t dht_raw[5];

/*===== [Prototypes (declarations) of private functions] =======*/

/*===== [Implementations of public functions] ==================*/
dht11_status_t dht11_read(dht11_data_t *data)
{
	dht11_status_t status;

	status = dht_check_presence();
	if (status != DHT11_OK)
	return status;

	status = dht_read_raw();
	timer0_stop();
	if (status != DHT11_OK)
	return status;

	/* checksum */
	if ((uint8_t)(dht_raw[0] + dht_raw[1] + dht_raw[2] + dht_raw[3]) != dht_raw[4])
	return DHT11_ERR_CHECKSUM;

	data->humidity_int    = dht_raw[0];
	data->humidity_dec    = dht_raw[1];
	data->temperature_int = dht_raw[2];
	data->temperature_dec = dht_raw[3];

	return DHT11_OK;
}

/*===== [Implementations of private functions] ==================*/
static inline void timer0_start(void)
{
	TCNT0 = 0;
	TCCR0B |= (1 << CS00);   // prescaler = 1 (ejemplo)
}

static inline void timer0_stop(void)
{
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}


static dht11_status_t dht_wait_level(uint8_t level, uint16_t timeout_us)
{
	TCNT0 = 0;

	while (((DHT_READ() != 0) != level)) {
		if (TCNT0 >= timeout_us) {
			return DHT11_ERR_TIMEOUT;
		}
	}

	return DHT11_OK;
}

static dht11_status_t dht_check_presence(void)
{
	/* START */
	DHT_SET_OUTPUT();
	DHT_LOW();
	_delay_ms(DHT_START_MS);
	DHT_HIGH();
	_delay_us(30);
	DHT_SET_INPUT();

	timer0_start();

	/* Sensor response */
	if (dht_wait_level(0, 100) != DHT11_OK) {
		timer0_stop();
		return DHT11_ERR_NO_RESPONSE;
	}

	if (dht_wait_level(1, 100) != DHT11_OK) {
		timer0_stop();
		return DHT11_ERR_NO_RESPONSE;
	}

	if (dht_wait_level(0, 100) != DHT11_OK) {
		timer0_stop();
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

			/* esperar inicio del bit (HIGH) */
			if (dht_wait_level(1, 70) != DHT11_OK)
			return DHT11_ERR_TIMEOUT;

			TCNT0 = 0;

			/* esperar fin del HIGH */
			if (dht_wait_level(0, 100) != DHT11_OK)
			return DHT11_ERR_TIMEOUT;

			uint8_t pulse_width = TCNT0;

			if (pulse_width > 40) {   // ~70 µs ? bit 1
				dht_raw[i] |= (1 << (7 - j));
			}
		}
	}

	return DHT11_OK;
}