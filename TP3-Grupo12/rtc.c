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

rtc_state_t rtc_get_time(rtc_time_t *time) {
    uint8_t reg_inicial = RTC_REG_SEC;

    if (twi_busy()) {
        return RTC_BUSY;
    }

    bool iniciado = twi_master_write_read(RTC_ADDR, &reg_inicial, 1, rtc_buffer, 3);
    
    if (!iniciado) {
        return RTC_NOT_INIT;
    }

    while (twi_busy());

    if (twi_done()) {
        time->sec = bcd_to_bin(rtc_buffer[0]);
        time->min  = bcd_to_bin(rtc_buffer[1]);
        time->hour    = bcd_to_bin(rtc_buffer[2] & 0x3F); 
        return RTC_OK;
    }
    return RTC_NOT_DONE; 
}

bool rtc_set_time(const rtc_time_t *time) {
    if (twi_busy()) {
        return false;
    }

    rtc_buffer[0] = RTC_REG_SEC;
    rtc_buffer[1] = bin_to_bcd(time->sec);
    rtc_buffer[2] = bin_to_bcd(time->min);
    rtc_buffer[3] = bin_to_bcd(time->hour) & 0x3F; 

    return twi_master_write(RTC_ADDR, rtc_buffer, 4);
}
