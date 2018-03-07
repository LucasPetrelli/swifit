/*
 * request_rules.c
 *
 *  Created on: Mar 6, 2018
 *      Author: Lucas
 */


#include "request_rules.h"
#include "webserver.h"
#include "devices.h"
#include "behavior_task.h"
#include "configuration.h"
#include "cJSON.h"

const char * zRequestRulesCGI = "request_rules";

char* zHandlerRequestRules(char* pcRequestData)
{
	char* acResponseBody = NULL;
	uint32 u32ResponseLen = 0;
	cJSON* jsList = cJSON_CreateArray();

	tsNetworkRule* psTable = psConfigurationGetRuleTable();
	uint8_t u8RuleIndex = 0;
	for (;u8RuleIndex < N_RULE_ENTRIES; u8RuleIndex++)
	{
		tsNetworkRule* psRule = &psTable[u8RuleIndex];
		if (psRule->eEnabled == RULE_ENABLED)
		{
			cJSON* psJSONRule = cJSON_CreateObject();
			char cID_[11];
			sprintf(cID_, "%u", psRule->u32TriggerId);
			cJSON_AddStringToObject(psJSONRule, "triggerid", cID_);
			sprintf(cID_, "%u", psRule->u32TargetId);
			cJSON_AddStringToObject(psJSONRule, "targetid", cID_);

			if (psRule->eTriggerState == TRIGGERED)
			{
				cJSON_AddTrueToObject(psJSONRule, "triggerevent");
			}
			else
			{
				cJSON_AddFalseToObject(psJSONRule, "triggerevent");
			}

			if (psRule->eTargetState == ACTIVATED)
			{
				cJSON_AddTrueToObject(psJSONRule, "targetact");
			}
			else
			{
				cJSON_AddFalseToObject(psJSONRule, "targetact");
			}
			cJSON_AddItemToArray(jsList, psJSONRule);
		}
	}
	acResponseBody = cJSON_Print(jsList);
	LOG_DEBUG("%s", acResponseBody);
	u32ResponseLen = strlen(zHttpHeaderOK)+strlen(acResponseBody)+1;
	char* pcResponse = (char*)zalloc(u32ResponseLen);
	strcat(pcResponse, zHttpHeaderOK);
	strcat(pcResponse, acResponseBody);
	free(acResponseBody);
	cJSON_Delete(jsList);
	return pcResponse;

}

