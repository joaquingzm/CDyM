/**
 * @file    driver_twi.h
 * @brief   -
 */

/*====[Avoid multiple inclusion - begin]=========================*/
#ifndef _DRIVER_TWI_
#define _DRIVER_TWI_

/*====[Inclusion of public function dependencies]================*/
#include <stdbool.h>
#include <stdint.h>

/*====[Definitions of public data types]=========================*/

/**
 * @brief Resultado de la ultima transaccion TWI completada.
 */
typedef enum {
    TWI_OK = 0,       
    TWI_ERR_NACK,      
    TWI_ERR_ARB_LOST,  
    TWI_ERR_BUS               
} twi_result_t;

/**
 * @brief -
 */
volatile bool TWI_DONE_FLAG;

/**
 * @brief -
 */
volatile bool TWI_ERROR_FLAG;

/*====[Prototypes (declarations) of public functions]============*/

/**
 * @brief   -
 */
void twi_init(uint32_t f_cpu, uint32_t scl_hz);

/**
 * @brief   -
 */
bool twi_busy(void);

/**
 * @brief   -
 */
bool twi_done(void);

/**
 * @brief   -
 */
bool twi_error(void);

/**
 * @brief   -
 */
twi_result_t twi_get_result(void);

/**
 * @brief   -
 */
bool twi_master_write(uint8_t addr, const uint8_t *data, uint8_t len);

/**
 * @brief   -
 */
bool twi_master_read(uint8_t addr, uint8_t *data, uint8_t len);

/**
 * @brief   -
 */
bool twi_master_write_read(uint8_t addr,
                           const uint8_t *wr, uint8_t wr_len,
                           uint8_t *rd,       uint8_t rd_len);

/*====[Avoid multiple inclusion - end]===========================*/
#endif /* _DRIVER_TWI_ */