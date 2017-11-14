/*
 * sensor.c
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */


#include "sensor.h"
#include "config.h"
#include "mem_types.h"

tsSensor asSensor[MAX_SENSOR];
tsSensorControl sSensorControl;

void vSensorInitPlatform(xQueueHandle xQueueFromISRToSensorTask)
{

	vGPIOIntrSetHandler(vSensorHandlerIsr);
	sSensorControl.xQueueFromISRToTask = xQueueFromISRToSensorTask;
	if (TARGET_PRODUCT == PLUG_PROD)
	{
		LOG_DEBUG("Config as PLUG");
		asSensor[0].u8Id = 1;
		asSensor[0].eType = PIR;
		asSensor[0].eGPIOAssigned = GPIO_4;
		asSensor[0].u8IntId = 0;

		vGPIOSetIOMode(GPIO_BIT(asSensor[0].eGPIOAssigned), GPIO_Input);
		vGPIOIntrSetTrigger(asSensor[0].eGPIOAssigned, GPIO_INTR_ANYEDGE);


		asSensor[1].eType = UNASSIGNED;
	}
	else
	{
		asSensor[0].eType = UNASSIGNED;
		asSensor[1].eType = UNASSIGNED;
	}
}

void vSensorHandlerIsr(uint32 mask)
{
	mask = u32GPIOIntrStatus();
	uint8 u8I;
	for (u8I = 0; u8I < MAX_SENSOR; u8I++)
	{
		if (GPIO_BIT(asSensor[u8I].eGPIOAssigned) & mask)
		{
			tsMemQueueMessage* psMsg = (tsMemQueueMessage*)zalloc(sizeof(tsMemQueueMessage));
			tsSensorEvent* psEvent = (tsSensorEvent*)zalloc(sizeof(tsSensorEvent));
			teGPIOValue eGpioState = eGPIORead(asSensor[u8I].eGPIOAssigned);

			psEvent->u8Id = asSensor[u8I].u8Id;
			if (eGpioState == HIGH)
			{
				psEvent->eState = ACTIVATED;
				asSensor[u8I].eState = ACTIVATED;
			}
			else
			{
				psEvent->eState = DEACTIVATED;
				asSensor[u8I].eState = DEACTIVATED;
			}
			psMsg->eType = SENSOR_MESSAGE;
			psMsg->pvData = (void*) psEvent;
			xQueueSendFromISR(sSensorControl.xQueueFromISRToTask ,(void*) psMsg, NULL);
			free(psMsg);

		}
	}
}
