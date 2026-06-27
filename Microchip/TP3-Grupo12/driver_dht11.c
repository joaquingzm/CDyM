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

#define DHT_START_MS    18
#define DHT_WAIT_US     200//140
#define DHT_BIT_DELAY   30

/*===== [Private function-like macros] ==========================*/
#define DHT_SET_OUTPUT()    (DHT_DDR |=  (1 << DHT_BIT))
#define DHT_SET_INPUT() (DHT_DDR &= ~(1 << DHT_BIT))
#define DHT_LOW()           (DHT_PORT &= ~(1 << DHT_BIT))
#define DHT_HIGH()          (DHT_PORT |=  (1 << DHT_BIT))
#define DHT_READ()          (DHT_PINR &   (1 << DHT_BIT)) 

/*===== [Definitions of private global variables] ===============*/
static uint8_t dht_raw[5]; //40bit de respuesta del sensor

/*===== [Prototypes (declarations) of private functions] =======*/
static inline void timer1_start(void);
static void timer1_stop(void);
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
	timer1_stop();
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

/*===== [Implementations of private functions] ==================*/
static inline void timer1_start(void)
{
	TCNT1 = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS11); // prescaler = 8
}

static inline void timer1_stop(void)
{
	TCCR1B = 0;
}


static dht11_status_t dht_wait_level(uint8_t level, uint16_t timeout_ticks)
{
	timer1_start();
	if (level) {
		while (!(PINC & (1 << PORTC0))) {
			if (TCNT1 >= timeout_ticks){ 
				timer1_stop(); 
				return DHT11_ERR_TIMEOUT;
			};
		}
		} else {
		while (PINC & (1 << PORTC0)) {
			if (TCNT1 >= timeout_ticks){
				timer1_stop();
				return DHT11_ERR_TIMEOUT;
			};
		}
	}
	return DHT11_OK;
}

static dht11_status_t dht_check_presence(void)
{

	//START
	DHT_SET_OUTPUT();	//Enviamos 18ms de low 
	DHT_LOW();
	_delay_ms(DHT_START_MS);
	DHT_SET_INPUT();	//Volvemos a setear pin como input
	_delay_us(30);

	 //Sensor response 
	if (dht_wait_level(0, 60) != DHT11_OK) {
		return DHT11_ERR_CHECKSUM;
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
	uint8_t aux=0;
	for (i = 0; i < 5; i++) {
		dht_raw[i] = 0;
	}

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 8; j++) {

			// esperar inicio del bit (HIGH) 
			
			
			if (dht_wait_level(1, 160) != DHT11_OK){
				aux= TCNT1;
				return DHT11_ERR_TIMEOUT;
			}
			// esperar fin del HIGH 
			if (dht_wait_level(0, 240) != DHT11_OK) return DHT11_ERR_TIMEOUT;

			uint8_t pulse_width = aux+TCNT1;

			if (pulse_width > 70) {   // ~70 µs ? bit 1
				dht_raw[i] |= (1 << (7 - j));
			}
		}
	}

	return DHT11_OK;
}