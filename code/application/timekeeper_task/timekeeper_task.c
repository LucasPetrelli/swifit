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
#include "devices.h"

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

		// Check the queue for an wifi message - it indicates a connection state change
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

		// If not connected to WLAN, the task should do nothing
		if (psTask->eState == TIMEKEEPER_WIFI_NOT_CONNECTED)
		{
			continue;
		}

		// Attempt to get the current timestamp. If fail, try to start the SNTP server connection
		uint32_t u32Timestamp = u32SNTPGetTimestamp();
		if (u32Timestamp == 0)
		{
			psTask->eState = TIMEKEEPER_NOT_SYNC;
			vSNTPInit();
			continue;
		}
		psTask->eState = TIMEKEEPER_SYNC;

		// Check if there is an event in the current time
		uint32_t u32Minutes = u32TimingMinutesFromTimestamp(u32Timestamp);
		tsDevice* psDev = psDeviceGetSelf();
		uint8_t u8Row;
		for (u8Row = 0; u8Row < N_TIME_ENTRIES; u8Row++)
		{
			tsTimingEntry* psRow = &(psDev->sTimeTable_[u8Row]);
			if (psRow->eEnabled == TIMING_DISABLED)
			{
				continue;
			}
			if ((uint16_t) u32Minutes == psRow->u16Minutes)
			{
				if (psRow->eAction == TIMING_ACTIVATE)
				{
					vActuatorTaskActivate(MAIN_RELAY);
				}
				else
				{
					vActuatorTaskDeactivate(MAIN_RELAY);
				}
			}
		}
		free(psDev);
	}
}
