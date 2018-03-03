/*
 * timing_task.c
 *
 *  Created on: Mar 2, 2018
 *      Author: Lucas
 */

#include "timekeeper_task.h"
#include "timing.h"
#include "debug_adapter.h"
#include "u_time.h"
#include "mem_types.h"
#include "wifi_types.h"

void vTimekeeperTask(void* pvParameters)
{
	tsTimekeeperTaskConfiguration* psTask = (tsTimekeeperTaskConfiguration*) pvParameters;
	psTask->xLastWakeUpTick = xTaskGetTickCount();
	psTask->eState = TIMEKEEPER_WIFI_NOT_CONNECTED;
	vSNTPInit();
	while (1)
	{
		vTaskDelayUntil(&psTask->xLastWakeUpTick, psTask->u32Period/portTICK_RATE_MS);
		tsMemQueueMessage sMsg;
		if (xQueueReceive(psTask->xTimekeeperQueue, (void*)&sMsg, 0) == pdTRUE)
		{
			LOG_DEBUG("Timekeeper received message");
			if (sMsg.eType == WIFI_MESSAGE)
			{
				teWifiConnectionStatus eNewWifiState = (teWifiConnectionStatus) sMsg.pvData;
				if (eNewWifiState == WIFI_DISCONNECTED)
				{
					LOG_DEBUG("Timekeeper wifi disconnected");
					psTask->eState = TIMEKEEPER_WIFI_NOT_CONNECTED;
				}
				if (eNewWifiState == WIFI_CONNECTED_TO_AP)
				{
					LOG_DEBUG("Timekeeper wifi connected");
					psTask->eState = TIMEKEEPER_NOT_SYNC;
				}
			}
		}

		if (psTask->eState == TIMEKEEPER_WIFI_NOT_CONNECTED)
		{
			continue;
		}

		uint32_t u32Timestamp = u32SNTPGetTimestamp();
		if (u32Timestamp == 0)
		{
			psTask->eState = TIMEKEEPER_NOT_SYNC;
			vSNTPInit();
			continue;
		}
		psTask->eState = TIMEKEEPER_SYNC;
		struct tmElements* psTM = (struct tmElements*)zalloc(sizeof(struct tmElements*));
		timet_to_tm((time_t) u32Timestamp, psTM);
		LOG_DEBUG(
				"%02u/%02u/%02u %02u:%02u:%02u",
				psTM->Day,
				psTM->Month,
				psTM->Year+1970,
				psTM->Hour,
				psTM->Minute,
				psTM->Second
		);
		free(psTM);
	}
}
