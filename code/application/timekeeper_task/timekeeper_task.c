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

void vTimekeeperTask(void* pvParameters)
{
	tsTimekeeperTaskConfiguration* psTask = (tsTimekeeperTaskConfiguration*) pvParameters;
	psTask->xLastWakeUpTick = xTaskGetTickCount();
	eWifiWaitToBeConnected(30);
	vSNTPInit();
	while (1)
	{
		vTaskDelayUntil(&psTask->xLastWakeUpTick, psTask->u32Period/portTICK_RATE_MS);
		uint32_t u32Timestamp = u32SNTPGetTimestamp();
		if (u32Timestamp == 0)
		{
			vSNTPInit();
			continue;
		}
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
