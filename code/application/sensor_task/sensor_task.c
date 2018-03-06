/*
 * sensor_task.c
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */

#include "sensor_task.h"
#include "mem_types.h"

void vSensorTask(void* pvParameters)
{
	xQueueHandle xQueueFromISRToTask = xQueueCreate(10, sizeof(tsMemQueueMessage));

	vSensorInitPlatform(xQueueFromISRToTask);

	while(1)
	{
		tsMemQueueMessage *psQueueMesssage = (tsMemQueueMessage *)zalloc(sizeof(tsMemQueueMessage));
		if (xQueueReceive(xQueueFromISRToTask ,(const void*) psQueueMesssage, portMAX_DELAY) == pdTRUE)
		{
			tsSensorEvent* psEvent = (tsSensorEvent*) psQueueMesssage->pvData;
			LOG_DEBUG(
				"Sensor [%u] status [%u]",
				psEvent->u8Id,
				(uint8)psEvent->eState
			);
			// Redirect event to behavior task

			tsNetworkEvent* psNetEvent = (tsNetworkEvent*)zalloc(sizeof(tsNetworkEvent));
			psNetEvent->eState = psEvent->eState;
			psNetEvent->u32Id = u32SystemGetId();
			free(psEvent);
			psQueueMesssage->pvData = (void*) psNetEvent;
			vBehaviourPutInQueue(psQueueMesssage);
		}
		free(psQueueMesssage);
	}
	vTaskDelay(portMAX_DELAY);

}
