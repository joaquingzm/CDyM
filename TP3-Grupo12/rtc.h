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
	uint8_t hour;   /* 0–23 */
	uint8_t min;    /* 0–59 */
	uint8_t sec;    /* 0–59 */
} rtc_time_t;

typedef enum {
    RTC_OK,
    RTC_BUSY,
	RTC_NOT_INIT,
	RTC_NOT_DONE
} rtc_state_t;

/**
 * @brief   -
 */
rtc_state_t rtc_get_time(rtc_time_t *time);

/**
 * @brief   -
 */
bool rtc_set_time(const rtc_time_t *time);

#endif /* _RTC_ */
