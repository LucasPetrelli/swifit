/*
 * update_rules.c
 *
 *  Created on: Mar 6, 2018
 *      Author: Lucas
 */




#include "update_rules.h"
#include "webserver.h"
#include "devices.h"
#include "behavior_task.h"
#include "cJSON.h"

const char * zUpdateRulesCGI = "update_rules";

char* zHandlerUpdateRules(char* pcRequestData)
{
	char* pcResponse;
	LOG_DEBUG("zHandlerUpdateRules");
	cJSON* psJSONNewRules = cJSON_Parse(pcRequestData);
	if (psJSONNewRules == NULL)
	{
		LOG_DEBUG("Handler didn't found JSON data");
		goto end_zHandlerUpdateRules;
	}
	tsNetworkRule* psNewRules = (tsNetworkRule*)zalloc(sizeof(tsNetworkRule)*N_RULE_ENTRIES);
	uint8_t u8AmountOfRules = cJSON_GetArraySize(psJSONNewRules);
	uint8_t u8RuleIndex = 0;
	LOG_DEBUG("Received %u rules", u8AmountOfRules);
	for (;u8RuleIndex < u8AmountOfRules; u8RuleIndex++)
	{
		cJSON* psJSONRule = cJSON_GetArrayItem(psJSONNewRules, u8RuleIndex);
		psNewRules[u8RuleIndex].eEnabled = RULE_ENABLED;
		psNewRules[u8RuleIndex].u32TriggerId = strtoul(cJSON_GetObjectItem(psJSONRule, "triggerid")->valuestring, NULL, 10);
		psNewRules[u8RuleIndex].u32TargetId = strtoul(cJSON_GetObjectItem(psJSONRule, "targetid")->valuestring, NULL, 10);
		psNewRules[u8RuleIndex].eTriggerState = (teSensorState) cJSON_GetObjectItem(psJSONRule, "triggerevent")->type;
		psNewRules[u8RuleIndex].eTargetState = (teActuatorState) cJSON_GetObjectItem(psJSONRule, "targetact")->type;
	}

	cJSON_Delete(psJSONNewRules);

	LOG_DEBUG("Sending to behavior");
	tsMemQueueMessage sMsg = {
			.eType = RULES_MESSAGE,
			.pvData = (void*) psNewRules
	};
	vBehaviourPutInQueue(&sMsg);


end_zHandlerUpdateRules:
	pcResponse = (char*)zalloc(strlen(zHttpHeaderOK));
	strcat(pcResponse, zHttpHeaderOK);
	return pcResponse;
}
