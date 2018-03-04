/*
 * devices.c
 *
 *  Created on: Feb 20, 2018
 *      Author: Lucas
 */


#include "devices_list.h"
#include "product_config.h"
#include "webserver.h"
#include "devices.h"
#include "behavior_task.h"
#include "cJSON.h"

const char* zDeviceListCGI = "devices_list";
char* zHandlerDeviceList(char* pcRequestData)
{
	char* acResponseBody = NULL;
	uint32 u32ResponseLen = 0;

	cJSON* jsData = cJSON_CreateObject();
	cJSON* jsList = cJSON_CreateArray();

	// create object to represent self
	tsDevice* psDevSelf = psDeviceGetSelf();
	cJSON* jsSelf = psWebserverMakeJSONFromDevice(psDevSelf);
	cJSON_AddItemToArray(jsList, jsSelf);
	free(psDevSelf);

	tsDevice* psKnownDevices = (tsDevice*) pxBehaviourGetListOfDevices();
	while(psKnownDevices)
	{
		cJSON* jsDev = psWebserverMakeJSONFromDevice(psKnownDevices);
		cJSON_AddItemToArray(jsList, jsDev);
		psKnownDevices = (tsDevice*) psKnownDevices->pvNext;
	}
	cJSON_AddItemToObject(jsData, "devices", jsList);

	acResponseBody = cJSON_Print(jsData);

	u32ResponseLen = strlen(zHttpHeaderOK)+strlen(acResponseBody)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);
	strcat(pcResponse, acResponseBody);
	free(acResponseBody);
	cJSON_Delete(jsData);
	return pcResponse;
}
