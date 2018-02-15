/*
 * platform_task.c
 *
 *  Created on: Feb 6, 2018
 *      Author: Lucas
 */

#include "platform_task.h"
#include "actuator_task.h"
#include "system_adapter.h"
#include "configuration.h"

portTickType xScheduledReset;

void vPlatformTask(void* pvParameters)
{
	xScheduledReset = 0;
	tsPlatformTaskConfiguration* psTask = (tsPlatformTaskConfiguration*) pvParameters;
	psTask->xLastWakeTime = xTaskGetTickCount();
	tsConfiguration* psConfig = psConfigurationGet();
	teOperationMode eMode = psConfig->eMode;
	vPlatformSetLEDs(eMode);

	while (1)
	{
		vTaskDelayUntil(&psTask->xLastWakeTime, psTask->u32Period/portTICK_RATE_MS );
		tsConfiguration* psConfig = psConfigurationGet();

		if (xScheduledReset)
		{
			if (psTask->xLastWakeTime > xScheduledReset)
			{

				LOG_DEBUG("Running scheduled reset");
				vSystemReset();
			}
		}

		if (psTask->u32HeapReportPeriod)
		{
			if (psTask->u32HeapReportCounter >= psTask->u32HeapReportPeriod)
			{
				psTask->u32HeapReportCounter = 0;

				uint32 u32Free = u32GetFreeHeap();
				LOG_DEBUG("Remaining heap: %u", u32Free);
			}

			psTask->u32HeapReportCounter += psTask->u32Period;
		}

		if (psConfig->eMode != eMode)
		{
			eMode = psConfig->eMode;
			vPlatformSetLEDs(eMode);
		}
	}
}

void vScheduleReset(uint32_t u32MillisDelay)
{
	portTickType xTick = xTaskGetTickCount();
	xScheduledReset = xTick + u32MillisDelay/portTICK_RATE_MS;
}

void vPlatformSetLEDs(teOperationMode eMode)
{
	switch (eMode)
	{
	case CONFIG_MODE:
	{
		vActuatorTaskToggleSlow(LED_BLUE, 0);
		vActuatorTaskDeactivate(LED_RED);
		break;
	}
	case NORMAL_MODE:
	{
		vActuatorTaskActivate(LED_BLUE);
		vActuatorTaskDeactivate(LED_RED);
		break;
	}
	case ATTEMPT_NEW_CONNECTION:
	{
		vActuatorTaskToggleFast(LED_BLUE, 0);
		vActuatorTaskDeactivate(LED_RED);
		break;
	}
	case CONNECTING_TO_KNOWN_AP:
	{
		vActuatorTaskToggleFast(LED_BLUE, 0);
		vActuatorTaskToggleFast(LED_RED, 0);
		break;
	}
	}
}
