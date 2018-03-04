/*
 * update_timing.c
 *
 *  Created on: Mar 3, 2018
 *      Author: Lucas
 */

#include "update_timing.h"
#include "devices_types.h"
#include "mem_types.h"
const char * zUpdateTimingCGI = "update_timing";

char* zHandlerUpdateTiming(char* pcRequestData)
{
	char* pcResponse;
	cJSON* psJSON = cJSON_Parse(pcRequestData);
	if (psJSON == NULL)
	{
		goto end_zHandlerUpdateTiming;
	}

	cJSON* psJSONId = cJSON_GetObjectItem(psJSON, "id");
	cJSON* psJSONTable = cJSON_GetObjectItem(psJSON, "table");

	tsDeviceTimingChange* psTable = (tsDeviceTimingChange*)zalloc(sizeof(tsDeviceTimingChange));
	psTable->u32Id = strtoul(psJSONId->valuestring, NULL, 10);

	uint8_t u8Row = 0;
	for (;u8Row < N_TIME_ENTRIES; u8Row++)
	{
		cJSON* psJSONEntry = cJSON_GetArrayItem(psJSONTable, u8Row);
		tsTimingEntry* psEntry = &(psTable->sTable_[u8Row]);

		if (cJSON_GetObjectItem(psJSONEntry, "enabled")->type == cJSON_True)
		{
			psEntry->eEnabled = TIMING_ENABLED;
		}
		else
		{
			psEntry->eEnabled = TIMING_DISABLED;
		}

		if (cJSON_GetObjectItem(psJSONEntry, "action")->type == cJSON_True)
		{
			psEntry->eAction = TIMING_ACTIVATE;
		}
		else
		{
			psEntry->eAction = TIMING_DEACTIVATE;
		}

		psEntry->u16Minutes = strtoul(cJSON_GetObjectItem(psJSONEntry, "time")->valuestring, NULL, 10);
	}
	cJSON_Delete(psJSON);

	// Make message to pass to behavior task
	tsMemQueueMessage sMsg = {
			.eType = TIMING_PARAMETER,
			.pvData = (void*) psTable
	};
	vBehaviourPutInQueue(&sMsg);


end_zHandlerUpdateTiming:
	pcResponse = (char*)zalloc(strlen(zHttpHeaderOK));
	strcat(pcResponse, zHttpHeaderOK);
	return pcResponse;
}
