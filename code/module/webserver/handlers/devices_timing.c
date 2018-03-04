/*
 * devices_timing.c
 *
 *  Created on: Mar 3, 2018
 *      Author: Lucas
 */

#include "devices_timing.h"
#include "devices.h"
#include "webserver.h"

const char * zDevicesTimingCGI = "devices_timing";

char* zHandlerDevicesTiming(char* pcRequestData)
{
	char* acResponseBody = NULL;
	uint32 u32ResponseLen = 0;
	LOG_DEBUG("Device timing handling");

	cJSON* jsList = cJSON_CreateArray();
	tsDevice* psDevSelf = psDeviceGetSelf();
	cJSON* jsSelf = psWebserverMakeJSONFromTiming(psDevSelf);
	free(psDevSelf);
	cJSON_AddItemToArray(jsList, jsSelf);

	// mount response
	acResponseBody = cJSON_Print(jsList);
	LOG_DEBUG("JSON Created: %s", acResponseBody);
	u32ResponseLen = strlen(zHttpHeaderOK)+strlen(acResponseBody)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);
	strcat(pcResponse, acResponseBody);
	free(acResponseBody);
	cJSON_Delete(jsList);
	return pcResponse;

}


