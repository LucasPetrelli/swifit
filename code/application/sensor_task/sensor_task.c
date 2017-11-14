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
	xQueueHandle xQueueFromSensorToTask= xQueueCreate(10, sizeof(tsMemQueueMessage));

	vSensorInitPlatform(xQueueFromSensorToTask);

	while(1)
	{
		tsMemQueueMessage *psQueueMesssage = (tsMemQueueMessage *)zalloc(sizeof(tsMemQueueMessage));
		if (xQueueReceive(xQueueFromSensorToTask ,(const void*) psQueueMesssage, portMAX_DELAY) == pdTRUE)
		{
			tsSensorEvent* psEvent = (tsSensorEvent*) psQueueMesssage->pvData;
			LOG_DEBUG(
				"Sensor [%u] status [%u]",
				psEvent->u8Id,
				(uint8)psEvent->eState
			);
			free(psQueueMesssage->pvData);
		}

		free(psQueueMesssage);
	}
	vTaskDelay(portMAX_DELAY);

}
