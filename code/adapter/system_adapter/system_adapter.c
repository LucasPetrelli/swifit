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
    vDebugInit();
    vSPIFFSInit();
}


uint32_t u32GetFreeHeap()
{
	return system_get_free_heap_size();
}


void vSystemReset()
{
	system_restart();
}
