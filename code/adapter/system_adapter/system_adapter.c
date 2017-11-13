/**
 * @file system_adapter.c
 * @author Lucas
 * @date Nov 9, 2017
 *
 * @brief
 */

#include "system_adapter.h"

void vSystemInit()
{
    UART_SetBaudrate(UART0, 115200);
    espconn_init();
}


void vTaskSystemHeapReport(void* pvParameters)
{
	while (1)
	{
		uint32 u32Free = system_get_free_heap_size();
		LOG_DEBUG("Remaining heap: %u", u32Free);
		vTaskDelay( 5000/portTICK_RATE_MS );
	}
}
