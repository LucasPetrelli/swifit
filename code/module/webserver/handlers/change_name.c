/*
 * change_name.c
 *
 *  Created on: Mar 1, 2018
 *      Author: Lucas
 */

#include "change_name.h"
#include "cJSON.h"

const char * zChangeNameCGI = "change_name";
char* zHandlerChangeName(char* pcRequestData)
{
	uint32_t u32ResponseLen = 0;
	LOG_DEBUG("Request data \n%s", pcRequestData);
	cJSON* psData = cJSON_Parse(pcRequestData);
	if (!psData)
	{
		LOG_DEBUG("%s (%s)", pcRequestData, cJSON_GetErrorPtr());
		goto end_zHandlerChangeName;
	}

	cJSON* psJSONId = cJSON_GetObjectItem(psData, "id");
	cJSON* psJSONName = cJSON_GetObjectItem(psData, "name");

	LOG_DEBUG(
			"Received request to change %s name to %s",
			psJSONId->valuestring,
			psJSONName->valuestring
	);

	cJSON_Delete(psData);
end_zHandlerChangeName:
	u32ResponseLen = strlen(zHttpHeaderOK)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);
	return pcResponse;
}
