/**
 * @file    rtc.h
 * @brief   -
 */

#ifndef _RTC_
#define _RTC_

#include <stdint.h>
#include <stdbool.h>

#define RTC_ADDR   0x68

#define RTC_REG_SEC    0x00
#define RTC_REG_MIN    0x01
#define RTC_REG_HOUR   0x02

typedef struct {
    uint8_t horas;
    uint8_t minutos;
    uint8_t segundos;
} rtc_time_t;

typedef enum {
    RTC_IDLE,
    RTC_BUSY_READING,
    RTC_BUSY_WRITING
} rtc_state_t;

/**
 * @brief   -
 */
bool rtc_get_time(rtc_time_t *time);

/**
 * @brief   -
 */
bool rtc_set_time(const rtc_time_t *time);

#endif /* _RTC_ */
