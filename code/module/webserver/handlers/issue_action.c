/*
 * issue_action.c
 *
 *  Created on: Feb 22, 2018
 *      Author: Lucas
 */

#include "issue_action.h"
#include "cJSON.h"
#include "actuator_task.h"
#include "mem_types.h"


const char * zIssueActionCGI =  "issue_action";
char* zHandlerIssueAction(char* pcRequestData)
{
	char* pcResponse;
	cJSON* jsAct = cJSON_Parse(pcRequestData);

	if (jsAct == NULL)
	{
		LOG_DEBUG("Failed issue_action request received");
		goto zHandlerIssueAction_end;
	}

	cJSON* jsTargetId = cJSON_GetObjectItem(jsAct, "target");
	cJSON* jsNewStatus = cJSON_GetObjectItem(jsAct, "status");
	uint32_t u32TargetId = strtoul(jsTargetId->valuestring, NULL, 10);
	tsActuatorTaskRequest* psReq;

	if (jsNewStatus->type == cJSON_True)
	{
		psReq = psActuatorMountRequest(RELAY, REQUEST_ACTIVATE);
	}
	else
	{
		psReq = psActuatorMountRequest(RELAY, REQUEST_DEACTIVATE);
	}
	psReq->u32Id = u32TargetId;

	tsMemQueueMessage sMsg;
	sMsg.eType = ACTUATOR_MESSAGE;
	sMsg.pvData = (void*) psReq;
	vBehaviourPutInQueue(&sMsg);

	cJSON_Delete(jsAct);

zHandlerIssueAction_end:
	pcResponse = (char*)zalloc(strlen(zHttpHeaderOK));
	strcat(pcResponse, zHttpHeaderOK);
	return pcResponse;
}
