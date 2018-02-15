/*
 * debug_adapter.c
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */



#include "debug_adapter.h"


xSemaphoreHandle xPrintSemaphore;

void vDebugInit(void)
{
	xPrintSemaphore = xSemaphoreCreateMutex();
}
