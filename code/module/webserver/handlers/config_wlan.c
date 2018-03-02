/*
 * config_wlan.c
 *
 *  Created on: Feb 4, 2018
 *      Author: Lucas
 */

#include "config_wlan.h"
#include "configuration.h"
#include "string.h"
#include "cJSON.h"

const char * zConfigWlanCGI =  "connect_wlan";
char* zHandlerConfigWlan(char* pcRequestData)
{
	const char* acResponseBody = NULL;
	uint32 u32ResponseLen = 0;
	cJSON* sObj = cJSON_Parse(pcRequestData);
	if (!sObj)
	{
		acResponseBody = "Error parsing data received!";
		LOG_DEBUG("%s (%s)", acResponseBody, cJSON_GetErrorPtr());
	}

	cJSON* jsonAP = cJSON_GetObjectItem(sObj, "WLAN");
	cJSON* jsonPSWD = cJSON_GetObjectItem(sObj, "PSWD");
	if (!jsonAP)
	{
		acResponseBody = "Error parsing AP received!";
		LOG_DEBUG("%s", acResponseBody);
	}
	if (!jsonPSWD)
	{
		acResponseBody = "Error parsing PSWD received!";
		LOG_DEBUG("%s", acResponseBody);
	}


	tsConfiguration sNewConfig;
	sNewConfig.eMode = ATTEMPT_NEW_CONNECTION;
	memset(sNewConfig.acAP_, '\0', strlen(sNewConfig.acAP_));
	strcpy(sNewConfig.acAP_, jsonAP->valuestring);
	memset(sNewConfig.acPassword_, '\0', strlen(sNewConfig.acPassword_));
	strcpy(sNewConfig.acPassword_, jsonPSWD->valuestring);

	LOG_DEBUG(
		"JSON data received:\n"
		"AP: %s\n"
		"PSWD: %s",
		sNewConfig.acAP_,
		sNewConfig.acPassword_
	);
	vConfigurationSet(&sNewConfig);


	acResponseBody = "Response ok!";


end_zHandlerConfigWlan:
	u32ResponseLen = strlen(zHttpHeaderOK)+strlen(acResponseBody)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);
	strcat(pcResponse, acResponseBody);
	return pcResponse;
}
