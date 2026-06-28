/**
 * @file    driver_twi.c
 * @brief   -
 */

/*====[Inclusion of own header]==================================*/
#include "driver_twi.h"

/*====[Inclusion of private function dependencies]===============*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

/*====[Definitions of private constants]=========================*/
#define TWI_ST_START        0x08
#define TWI_ST_REP_START    0x10
#define TWI_ST_SLA_W_ACK    0x18
#define TWI_ST_SLA_W_NACK   0x20
#define TWI_ST_DATA_W_ACK   0x28
#define TWI_ST_DATA_W_NACK  0x30
#define TWI_ST_ARB_LOST     0x38
#define TWI_ST_SLA_R_ACK    0x40
#define TWI_ST_SLA_R_NACK   0x48
#define TWI_ST_DATA_R_ACK   0x50
#define TWI_ST_DATA_R_NACK  0x58

/*====[Private function-like macros]=============================*/
#define TWI_SEND_START() (TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE))
#define TWI_SEND_STOP() (TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN))
#define TWI_SEND_ACK() (TWCR = (1<<TWINT)|(1<<TWEA) |(1<<TWEN)|(1<<TWIE))
#define TWI_SEND_NACK() (TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE))
#define TWI_NEXT() (TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE))

/*====[Definitions of private global variables]==================*/
typedef enum {
    TWI_IDLE,
    TWI_WRITING,
    TWI_READING
} twi_mode_t;

static volatile twi_mode_t   mode   = TWI_IDLE;
static volatile twi_result_t result = TWI_OK;

static uint8_t sla;

static const uint8_t        *tx_buf;
static volatile uint8_t      tx_len;
static volatile uint8_t      tx_idx;

static uint8_t               *rx_buf;
static volatile uint8_t       rx_len;
static volatile uint8_t       rx_idx;

/*====[Prototypes (declarations) of private functions]===========*/
static void twi_finish_ok(void);
static void twi_finish_error(twi_result_t err);

/*====[Implementations of public functions]======================*/
void twi_init(uint32_t f_cpu, uint32_t scl_hz)
{
    TWSR = 0x00;
    TWBR = (uint8_t)((f_cpu / scl_hz - 16UL) / 2UL);

    TWI_DONE_FLAG  = false;
    TWI_ERROR_FLAG = false;
    mode           = TWI_IDLE;
    result         = TWI_OK;

    TWCR = (1<<TWEN); 
}

bool twi_busy(void)
{
    return mode != TWI_IDLE;
}

bool twi_done(void)
{
    cli();
    bool f = TWI_DONE_FLAG;
    TWI_DONE_FLAG = false;
    sei();
    return f;
}

bool twi_error(void)
{
    cli();
    bool f = TWI_ERROR_FLAG;
    TWI_ERROR_FLAG = false;
    sei();
    return f;
}

twi_result_t twi_get_result(void)
{
    cli();
    twi_result_t r = result;
    sei();
    return r;
}

bool twi_master_write(uint8_t addr, const uint8_t *data, uint8_t len)
{
    cli();
    if(mode != TWI_IDLE) { sei(); return false; }

    sla    = addr;
    tx_buf = data;
    tx_len = len;
    tx_idx = 0;
    rx_buf = NULL; 
    rx_len = 0;
    rx_idx = 0;
    mode   = TWI_WRITING;

    TWI_SEND_START();
    sei();
    return true;
}

bool twi_master_read(uint8_t addr, uint8_t *data, uint8_t len)
{
    if(len == 0) return false;

    cli();
    if(mode != TWI_IDLE) { sei(); return false; }

    sla    = addr;
    tx_buf = NULL;
    tx_len = 0;
    tx_idx = 0;
    rx_buf = data;
    rx_len = len;
    rx_idx = 0;
    mode   = TWI_READING;

    TWI_SEND_START();
    sei();
    return true;
}

bool twi_master_write_read(uint8_t addr,
                           const uint8_t *wr, uint8_t wr_len,
                           uint8_t *rd,       uint8_t rd_len)
{
    if(rd_len == 0) return false;

    cli();
    if(mode != TWI_IDLE) { sei(); return false; }

    sla    = addr;
    tx_buf = wr;
    tx_len = wr_len;
    tx_idx = 0;
    rx_buf = rd;    
    rx_len = rd_len;
    rx_idx = 0;
    mode   = TWI_WRITING;  

    TWI_SEND_START();
    sei();
    return true;
}

/*====[Implementations of private functions]=====================*/
static void twi_finish_ok(void)
{
    TWI_SEND_STOP();
    result = TWI_OK;
    TWI_DONE_FLAG = true;
    mode = TWI_IDLE;
}

static void twi_finish_error(twi_result_t err)
{
    TWI_SEND_STOP();
    result         = err;
    TWI_ERROR_FLAG = true;
    mode           = TWI_IDLE;
}

/*====[Implementations of ISRs]==================================*/
ISR(TWI_vect)
{
    switch(TWSR & 0xF8)
    {
        case TWI_ST_START:
        case TWI_ST_REP_START:
            TWDR = (mode == TWI_READING)
                   ? ((sla << 1) | 0x01)    /* SLA+R */
                   : ((sla << 1) | 0x00);   /* SLA+W */
            TWI_NEXT();
            break;

        case TWI_ST_SLA_W_ACK:
        case TWI_ST_DATA_W_ACK:
            if(tx_idx < tx_len)
            {
                TWDR = tx_buf[tx_idx++];
                TWI_NEXT();
            }
            else if(rx_buf != NULL)
            {
                mode = TWI_READING;
                TWI_SEND_START();
            }
            else
            {
                twi_finish_ok();
            }
            break;

        case TWI_ST_SLA_W_NACK:
        case TWI_ST_DATA_W_NACK:
        case TWI_ST_SLA_R_NACK:
            twi_finish_error(TWI_ERR_NACK);
            break;

        case TWI_ST_ARB_LOST:
            TWCR           = (1<<TWEN);   
            result         = TWI_ERR_ARB_LOST;
            TWI_ERROR_FLAG = true;
            mode           = TWI_IDLE;
            break;

        case TWI_ST_SLA_R_ACK:
            if(rx_len > 1)
                TWI_SEND_ACK();
            else
                TWI_SEND_NACK();
            break;

        case TWI_ST_DATA_R_ACK:
            rx_buf[rx_idx++] = TWDR;
            if(rx_idx < rx_len - 1)
                TWI_SEND_ACK();
            else
                TWI_SEND_NACK();
            break;

        case TWI_ST_DATA_R_NACK:
            rx_buf[rx_idx++] = TWDR;
            twi_finish_ok();
            break;

        default:
            twi_finish_error(TWI_ERR_BUS);
            break;
    }
}