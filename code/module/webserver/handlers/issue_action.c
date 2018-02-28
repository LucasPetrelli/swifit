/*
 * issue_action.c
 *
 *  Created on: Feb 22, 2018
 *      Author: Lucas
 */

#include "issue_action.h"
#include "cJSON.h"
#include "actuator_task.h"


const char * zIssueActionCGI =  "issue_action";
char* zHandlerIssueAction(char* pcRequestData)
{
	cJSON* jsAct = cJSON_Parse(pcRequestData);
	cJSON* jsNewStatus = cJSON_GetObjectItem(jsAct, "status");

	if (jsNewStatus->type == cJSON_True)
	{
		vActuatorTaskActivate(RELAY);
	}
	else
	{
		vActuatorTaskDeactivate(RELAY);
	}

	cJSON_Delete(jsAct);

	char* pcResponse = (char*)zalloc(strlen(zHttpHeaderOK));
	strcat(pcResponse, zHttpHeaderOK);
	return pcResponse;
}
