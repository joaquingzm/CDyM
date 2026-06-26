/**
 * @file monitor.c
 * @brief Monitor de invernadero.
 *        Posee y conduce: terminal, timer, driver_rtc, driver_dht11.
 */

/*====[Inclusion of own header]==================================*/
#include "monitor.h"

/*====[Inclusion of private function dependencies]===============*/
#include "terminal.h"
#include "timer.h"
#include "driver_rtc.h"
#include "driver_dht11.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/*====[Definitions]==============================================*/
#define REPORT_INTERVAL_DEFAULT   5
#define REPORT_INTERVAL_MIN       2
#define REPORT_INTERVAL_MAX      60
#define ALERT_PERIOD_FRAMES       2   // trama de alerta cada N tramas de telemetría

// Ventana horaria
#define DAY_HOUR_START    7
#define DAY_HOUR_END     18

// Rangos día (07:00–18:59)
#define DAY_TEMP_MIN     20
#define DAY_TEMP_MAX     30
#define DAY_HUM_MIN      50
#define DAY_HUM_MAX      70

// Rangos noche (19:00–06:59)
#define NIGHT_TEMP_MIN   15
#define NIGHT_TEMP_MAX   22
#define NIGHT_HUM_MIN    60
#define NIGHT_HUM_MAX    80

// Debe coincidir con MSG_SIZE de terminal.c
#define DISPLAY_MSG_SIZE 32

/*====[Private state]============================================*/
static uint8_t report_interval     = REPORT_INTERVAL_DEFAULT;
static uint8_t report_counter      = 0;
static uint8_t alert_frame_counter = 0;
static char    display_msg[DISPLAY_MSG_SIZE];

/*====[Private prototypes]=======================================*/
static void monitor_report(void);
static void monitor_parse_command(const char *cmd);
static bool is_daytime(const rtc_time_t *t);
static bool check_temp_ok(int8_t temp, bool day);
static bool check_hum_ok(uint8_t hum, bool day);

/*====[Public functions]=========================================*/
void monitor_init(void)
{
    terminal_init(F_CPU, "Iniciando monitor...");
    timer_init();
    rtc_init();
    dht11_init();

    report_interval     = REPORT_INTERVAL_DEFAULT;
    report_counter      = 0;
    alert_frame_counter = 0;

    strncpy(display_msg, "Sin datos", sizeof(display_msg) - 1);
    display_msg[sizeof(display_msg) - 1] = '\0';
}

void monitor_dispatch(void)
{
    terminal_dispatch();

    if(terminal_line_available())
    {
        char cmd[64];
        if(terminal_get_line(cmd, sizeof(cmd)))
            monitor_parse_command(cmd);
    }

    if(FLAG_TIMER)
    {
        FLAG_TIMER = 0;
        report_counter++;
        if(report_counter >= report_interval)
        {
            report_counter = 0;
            monitor_report();
        }
    }
}

/*====[Private functions]========================================*/
static void monitor_report(void)
{
    rtc_time_t   t;
    dht11_data_t d;

    bool rtc_ok = rtc_get_time(&t);
    bool dht_ok = dht11_read(&d);

    bool day     = rtc_ok ? is_daytime(&t) : true;
    bool temp_ok = (rtc_ok && dht_ok) ? check_temp_ok(d.temp, day) : true;
    bool hum_ok  = (rtc_ok && dht_ok) ? check_hum_ok(d.hum,  day) : true;
    bool alert   = !temp_ok || !hum_ok;

    if(!rtc_ok && !dht_ok)
    {
        strncpy(display_msg, "Sin RTC | Sin sensor", sizeof(display_msg) - 1);
    }
    else if(!rtc_ok)
    {
        snprintf(display_msg, sizeof(display_msg),
                 "Sin RTC | T:%dC H:%d%%", d.temp, d.hum);
    }
    else if(!dht_ok)
    {
        snprintf(display_msg, sizeof(display_msg),
                 "[%02d:%02d:%02d] Sin sensor",
                 t.hour, t.min, t.sec);
    }
    else if(alert && (alert_frame_counter == ALERT_PERIOD_FRAMES - 1))
    {
        if(!temp_ok)
            snprintf(display_msg, sizeof(display_msg),
                     "[ALERTA] %02d:%02d Temp! %dC",
                     t.hour, t.min, d.temp);
        else
            snprintf(display_msg, sizeof(display_msg),
                     "[ALERTA] %02d:%02d Hum! %d%%",
                     t.hour, t.min, d.hum);
    }
    else
    {
        snprintf(display_msg, sizeof(display_msg),
                 "[%02d:%02d:%02d]T:%dC H:%d%% %s",
                 t.hour, t.min, t.sec,
                 d.temp, d.hum,
                 alert ? "ALERTA" : "NORMAL");
    }

    display_msg[sizeof(display_msg) - 1] = '\0';

    if(alert)
        alert_frame_counter = (alert_frame_counter + 1) % ALERT_PERIOD_FRAMES;
    else
        alert_frame_counter = 0;

    terminal_show_msg(display_msg);
}

static void monitor_parse_command(const char *cmd)
{
    char resp[DISPLAY_MSG_SIZE];

    if(strncmp(cmd, "SET_TIME=", 9) == 0)
    {
        const char *p = cmd + 9;

        if(strlen(p) < 8 || p[2] != ':' || p[5] != ':')
        {
            terminal_show_msg("Formato:HH:MM:SS");
            return;
        }

        if(p[0]<'0'||p[0]>'9'||p[1]<'0'||p[1]>'9'||
           p[3]<'0'||p[3]>'9'||p[4]<'0'||p[4]>'9'||
           p[6]<'0'||p[6]>'9'||p[7]<'0'||p[7]>'9')
        {
            terminal_show_msg("Hora invalida");
            return;
        }

        uint8_t h = (uint8_t)((p[0]-'0')*10 + (p[1]-'0'));
        uint8_t m = (uint8_t)((p[3]-'0')*10 + (p[4]-'0'));
        uint8_t s = (uint8_t)((p[6]-'0')*10 + (p[7]-'0'));

        if(h > 23 || m > 59 || s > 59)
        {
            terminal_show_msg("Hora invalida");
            return;
        }

        rtc_time_t new_time = {h, m, s};
        if(rtc_set_time(&new_time))
        {
            snprintf(resp, sizeof(resp), "Hora:%02d:%02d:%02d", h, m, s);
            terminal_show_msg(resp);
        }
        else
        {
            terminal_show_msg("Error RTC");
        }
    }
    else if(strncmp(cmd, "SET_TM=", 7) == 0)
    {
        const char *p = cmd + 7;
        uint8_t val = 0;

		while(*p >= '0' && *p <= '9')
        {
            val = (uint8_t)(val * 10 + (*p - '0'));
            p++;
        }

        if(*p != '\0')
        {
            terminal_show_msg("Formato: SET_TM=SS");
            return;
        }

        if(val < REPORT_INTERVAL_MIN || val > REPORT_INTERVAL_MAX)
        {
            snprintf(resp, sizeof(resp),
                     "Rango:%d-%ds",
                     REPORT_INTERVAL_MIN, REPORT_INTERVAL_MAX);
            terminal_show_msg(resp);
            return;
        }

        report_interval = val;
        report_counter  = 0;  // próximo reporte en exactamente val segundos
        snprintf(resp, sizeof(resp), "Intervalo:%ds", val);
        terminal_show_msg(resp);
    }
    else
    {
        terminal_show_msg("Cmd desconocido");
    }
}

static bool is_daytime(const rtc_time_t *t)
{
    return (t->hour >= DAY_HOUR_START) && (t->hour <= DAY_HOUR_END);
}

static bool check_temp_ok(int8_t temp, bool day)
{
    if(day)
        return (temp >= DAY_TEMP_MIN)   && (temp <= DAY_TEMP_MAX);
    else
        return (temp >= NIGHT_TEMP_MIN) && (temp <= NIGHT_TEMP_MAX);
}

static bool check_hum_ok(uint8_t hum, bool day)
{
    if(day)
        return (hum >= DAY_HUM_MIN)   && (hum <= DAY_HUM_MAX);
    else
        return (hum >= NIGHT_HUM_MIN) && (hum <= NIGHT_HUM_MAX);
}