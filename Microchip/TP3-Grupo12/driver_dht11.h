#ifndef DRIVER_DHT11_H
#define DRIVER_DHT11_H

#include <stdint.h>

/*===== [Public types] ========================================*/

typedef enum {
	DHT11_OK = 0,
	DHT11_ERR_NO_RESPONSE,
	DHT11_ERR_TIMEOUT,
	DHT11_ERR_CHECKSUM
} dht11_status_t;

typedef struct {
	uint8_t humidity_int;
	uint8_t humidity_dec;
	uint8_t temperature_int;
	uint8_t temperature_dec;
} dht11_data_t;

/*===== [Public function prototypes] ===========================*/

dht11_status_t dht11_read(dht11_data_t *data);

char *dht11_ToString(dht11_data_t data);

#endif /* DRIVER_DHT11_H */