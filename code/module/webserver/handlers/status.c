/*
 * status.c
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */
#include "status.h"

#include "freertos/FreeRTOS.h"
const char * zStatusCGI =  "status";
char* zHandlerStatus(char* pcRequestData)
{
	char acResponseBody[128];
	uint32_t u32ResponseLen;
	sprintf(acResponseBody, "%u ms up time", xTaskGetTickCount()*portTICK_RATE_MS);

end_zHandlerStatus:
	u32ResponseLen = strlen(zHttpHeaderOK)+strlen(acResponseBody)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);
	strcat(pcResponse, acResponseBody);
	return pcResponse;
}
