/*
 * config_message.c
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */


#include "config_message.h"
#include "configuration.h"

const char * zConfigMessageCGI = "config_message";

char* zHandlerConfigMessage(char* pcRequestData)
{
	tsConfiguration* sConfig = psConfigurationGet();
	uint32_t u32ResponseLen = strlen(zHttpHeaderOK)+strlen(sConfig->acMessage_)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);
	strcat(pcResponse, sConfig->acMessage_);

	vConfigurationClearMessage();
	return pcResponse;
}
