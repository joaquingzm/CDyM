/*
 * sistema.c
 */ 

/*====[Inclusion of own header]==================================*/
#include "sistema.h"

#define F_CPU 16000000UL

/*====[Inclusion of private function dependencies]===============*/
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "terminal.h"
#include "timer2.h"
#include "ldr.h"
#include "ledRGB.h"

/*====[Definitions of private global variables]==================*/
static uint8_t red_val;
static uint8_t green_val;
static uint8_t blue_val;
static uint16_t periodo_ldr;

typedef enum {
	ESTADO_SUBIENDO,
	ESTADO_MANTENIENDO,
	ESTADO_BAJANDO,
	ESTADO_APAGADO
} efecto_estado_t;

/*====[Prototypes (declarations) of private functions]===========*/
static void actualizar_efecto(uint32_t ms_actuales);
static void process_command(const char *cmd);

/*====[Implementations of public functions]======================*/
void sistema_init()
{
	// Iniciar terminal
	terminal_init(F_CPU, "");
	
	// Iniciar ledRGB
	ledRGB_init();
	ledRGB_set(0,255,0);
	
	// Iniciar ldr
	ldr_init();
	
	// Iniciar timer para el periodo de ledRGB
	timer2_init();
	
	// Inicialización de variables privadas globales
	red_val = 50;
	green_val = 50;
	blue_val = 50;
	periodo_ldr = 3000;
	
}


void sistema_dispatch(void)
{
	static char cmd[64];
	// Terminal
	terminal_dispatch();
	if(terminal_line_available())
	{
		terminal_get_line(cmd, sizeof(cmd));
		process_command(cmd);
	}

	static uint16_t count = 0;
	if(FLAG_TIMER)
	{
		FLAG_TIMER = 0;
		count++;
		actualizar_efecto(count*10);
		if(count*10 >= periodo_ldr)
		{
			count = 0;
			uint16_t ldr = ldr_read();
			periodo_ldr = 6000 - (((uint32_t)ldr*3000)/1023);
		}
	}
}

/*====[Implementations of private functions]=====================*/
/*
Devuelve puntero al char siguiente al numero parseado, o NULL si invalido.
Acepta 1 a 3 digitos, valor 0-255.
*/
static const char *parse_byte(const char *p, uint8_t *out)
{
    if(*p < '0' || *p > '9') return NULL;

    uint16_t val = 0;
    uint8_t ndigits = 0;

    while(*p >= '0' && *p <= '9')
    {
		// Mas de 3 digitos: invalido 
        if(ndigits == 3) return NULL;         
        val = (uint16_t)(val * 10 + (uint8_t)(*p - '0'));
		// corta apenas se pasa
        if(val > 255) return NULL;             
        p++;
        ndigits++;
    }

    *out = (uint8_t)val;
    return p;
}

static void process_command(const char *cmd)
{
	uint8_t r, g, b;
	if(strncmp(cmd, "SET_COLOR=", 10) == 0)
    {
        const char *p = cmd + 10;
       

        p = parse_byte(p, &r);
        if(p == NULL || *p != ',')
        {
           // terminal_show_msg("Formato:R,G,B");
            return;
        }
        p++;

        p = parse_byte(p, &g);
        if(p == NULL || *p != ',')
        {
            //terminal_show_msg("Formato:R,G,B");
            return;
        }
        p++;

        p = parse_byte(p, &b);
        if(p == NULL || *p != '\0')
        {
            //terminal_show_msg("Formato:R,G,B");
            return;
        }

        red_val = r;
		green_val = g;
		blue_val = b;
		
		//terminal_show_msg("\nCargaste valores");
    }
    else
    {
        //terminal_show_msg("\nCmd desconocido");
    }
	
	
}

static void actualizar_efecto(uint32_t ms_actuales) 
{
	static efecto_estado_t estado = ESTADO_SUBIENDO;
	static uint32_t ms_inicio_estado = 0;
	
	// Tiempo dentro del estado actual
	uint32_t t_estado = ms_actuales - ms_inicio_estado; 

	switch (estado) {
		case ESTADO_SUBIENDO:
			if (t_estado <= 1000)
			{
				// Sube proporcionalmente al tiempo transcurrido
				uint8_t r = (red_val * t_estado) / 1000;
				uint8_t g = (green_val * t_estado) / 1000;
				uint8_t b = (blue_val * t_estado) / 1000;
				ledRGB_set(r, g, b);
			} 
			else
			{
				estado = ESTADO_MANTENIENDO;
				ms_inicio_estado = ms_actuales;
			}
		break;

		case ESTADO_MANTENIENDO:
			if (t_estado <= 1000) 
			{
				// Asegura nivel máximo
				ledRGB_set(red_val, green_val, blue_val);
			} 
			else 
			{
				estado = ESTADO_BAJANDO;
				ms_inicio_estado = ms_actuales;
			}
		break;

		case ESTADO_BAJANDO:
			if (t_estado <= 1000)
			{
				// Baja proporcionalmente: (1000 - t_estado) invierte la pendiente
				uint8_t r = (red_val * (1000 - t_estado)) / 1000;
				uint8_t g = (green_val * (1000 - t_estado)) / 1000;
				uint8_t b = (blue_val * (1000 - t_estado)) / 1000;
				ledRGB_set(r, g, b);
			} 
			else
			{
				estado = ESTADO_APAGADO;
				ms_inicio_estado = ms_actuales;
			}
		break;

		case ESTADO_APAGADO:
			ledRGB_set(0, 0, 0); // Apagado
		
			// Acá entra el LDR: El ciclo total (T) es dictado por el sensor.
			// Los 3 estados anteriores consumieron 3000 ms.
			// Si el periodo_ldr es 4000ms, debe estar apagado (4000 - 3000) = 1000ms.
			if (t_estado >= (periodo_ldr - 3000))
			{
				estado = ESTADO_SUBIENDO;
				ms_inicio_estado = ms_actuales;
			}
		break;
	}
}