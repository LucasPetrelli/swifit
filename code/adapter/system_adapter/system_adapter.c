/**
 * @file system_adapter.c
 * @author Lucas
 * @date Nov 9, 2017
 *
 * @brief
 */

#include "system_adapter.h"
#include "math.h"

void vSystemInit()
{
    UART_SetBaudrate(UART0, 230400);
    espconn_init();
    vDebugInit();
    vSPIFFSInit();
    LOG_DEBUG("Id: %u", u32SystemGetId);
}


uint32_t u32SystemFreeHeap()
{
	return system_get_free_heap_size();
}


void vSystemReset()
{
	system_restart();
}

uint32_t u32SystemUptime()
{
	return (uint32_t) system_get_time();
}

uint32_t u32SystemGetId()
{
	uint8_t pu8Mac[6];
	uint32_t u32Id = 0;
	if (wifi_get_macaddr(STATION_IF, pu8Mac))
	{
		u32Id = ((uint32_t)pu8Mac[0]) | \
				((uint32_t)pu8Mac[1])<<8 | \
				((uint32_t)pu8Mac[4])<<16 | \
				((uint32_t)pu8Mac[5])<<24;
	}
	return u32Id;
}

uint32_t u32SystemRandom()
{
	return *(volatile uint32_t *)0x3FF20E44;
}
