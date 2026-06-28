/**
 * @file    rtc.c
 * @brief   -
 */

#include "rtc.h"
#include "driver_twi.h" 

static uint8_t rtc_buffer[4];

static uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

static uint8_t bin_to_bcd(uint8_t bin) {
    return ((bin / 10) << 4) | (bin % 10);
}

bool rtc_get_time(rtc_time_t *time) {
    uint8_t reg_inicial = RTC_REG_SEC;

    if (twi_busy()) {
        return false;
    }

    bool iniciado = twi_master_write_read(RTC_ADDR, &reg_inicial, 1, rtc_buffer, 3);
    
    if (!iniciado) {
        return false;
    }

    while (twi_busy());

    if (twi_done()) {
        time->segundos = bcd_to_bin(rtc_buffer[0]);
        time->minutos  = bcd_to_bin(rtc_buffer[1]);
        time->horas    = bcd_to_bin(rtc_buffer[2] & 0x3F); 
        return true;
    }
    return false; 
}

bool rtc_set_time(const rtc_time_t *time) {
    if (twi_busy()) {
        return false;
    }

    rtc_buffer[0] = RTC_REG_SEC;
    rtc_buffer[1] = bin_to_bcd(time->segundos);
    rtc_buffer[2] = bin_to_bcd(time->minutos);
    rtc_buffer[3] = bin_to_bcd(time->horas) & 0x3F; 

    return twi_master_write(RTC_ADDR, rtc_buffer, 4);
}
