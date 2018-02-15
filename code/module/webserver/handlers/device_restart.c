/*
 * device_restart.c
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */


#include "device_restart.h"
#include "platform_task.h"



const char * zDeviceRestartCGI = "device_restart";

char* zHandlerDeviceRestart(char* pcRequestData)
{
	uint32_t u32ResponseLen = strlen(zHttpHeaderOK)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);

	vScheduleReset(1000);

	return pcResponse;
}
