/*
 * actuator_task.c
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */


#include "actuator_task.h"
#include "mem_types.h"


xQueueHandle xQueue;


void vActuatorTask(void* pvParameters)
{
	vActuatorInitPlatform();
	xQueue = xQueueCreate(10, sizeof(tsMemQueueMessage));

	const uint8_t u8TaskPeriod = 100;
	const uint8_t u8FastPeriod = 1;
	const uint8_t u8SlowPeriod = 5;

	tsActuatorTaskRequest sCurrentRequest[N_ACTUATORS];
	while(1)
	{
		// See if a new request is available
		tsMemQueueMessage* psQueueMessage = (tsMemQueueMessage*) zalloc(sizeof(tsMemQueueMessage));
		while (xQueueReceive(xQueue, (const void*) psQueueMessage, 0) == pdTRUE)
		{
			tsActuatorTaskRequest* psRequest = (tsActuatorTaskRequest*) psQueueMessage->pvData;
			sCurrentRequest[psRequest->eId] = *psRequest;
			sCurrentRequest[psRequest->eId].eStatus = REQUEST_UNTREATED;
			free(psQueueMessage->pvData);
		}
		free(psQueueMessage);
		// check if there are requests under treatment
		teActuatorId eActuator;
		for (eActuator = 0; eActuator < N_ACTUATORS; eActuator++)
		{
			if (sCurrentRequest[eActuator].eStatus == REQUEST_TREATED)
			{
				continue;
			}
			switch (sCurrentRequest[eActuator].eType)
			{
				case REQUEST_FLASH_FAST_TIMED:
				{
					if (sCurrentRequest[eActuator].u8Duration == 0)
					{
						sCurrentRequest[eActuator].eStatus = REQUEST_TREATED;
						vActuatorDeactivateById(eActuator);
						break;
					}
					if (sCurrentRequest[eActuator].u8Duration % u8FastPeriod == 0)
					{
						vActuatorToggleById(eActuator);
					}
					sCurrentRequest[eActuator].u8Duration -= 1;
					break;
				}
				case REQUEST_FLASH_FAST_STEADY:
				{
					if (sCurrentRequest[eActuator].u8Duration == 0)
					{
						vActuatorToggleById(eActuator);
						sCurrentRequest[eActuator].u8Duration = u8FastPeriod;
					}
					sCurrentRequest[eActuator].u8Duration -= 1;
					break;
				}
				case REQUEST_FLASH_SLOW_TIMED:
				{
					if (sCurrentRequest[eActuator].u8Duration == 0)
					{
						sCurrentRequest[eActuator].eStatus = REQUEST_TREATED;
						vActuatorDeactivateById(eActuator);
						break;
					}
					sCurrentRequest[eActuator].u8Duration -= 1;
					if (sCurrentRequest[eActuator].u8Duration % u8SlowPeriod == 0)
					{
						vActuatorToggleById(eActuator);
					}
					break;
				}
				case REQUEST_FLASH_SLOW_STEADY:
				{
					if (sCurrentRequest[eActuator].u8Duration == 0)
					{
						vActuatorToggleById(eActuator);
						sCurrentRequest[eActuator].u8Duration = u8SlowPeriod;
					}
					sCurrentRequest[eActuator].u8Duration -= 1;
					break;
				}
				case REQUEST_ACTIVATE:
				{
					if (sCurrentRequest[eActuator].eStatus == REQUEST_UNTREATED)
					{
						vActuatorActivateById(eActuator);
						if (sCurrentRequest[eActuator].u8Duration == 0)
						{
							sCurrentRequest[eActuator].eStatus = REQUEST_TREATED;
						}
						else
						{
							sCurrentRequest[eActuator].eStatus = REQUEST_UNDER_TREATMENT;
						}
					}
					else
					{
						sCurrentRequest[eActuator].u8Duration -= 1;
						if (sCurrentRequest[eActuator].u8Duration == 0)
						{
							vActuatorDeactivateById(eActuator);
							sCurrentRequest[eActuator].eStatus = REQUEST_TREATED;
						}
					}
					break;
				}
				case REQUEST_DEACTIVATE:
				{
					vActuatorDeactivateById(eActuator);
					sCurrentRequest[eActuator].eStatus = REQUEST_TREATED;
					break;
				}
				default:
				{
					sCurrentRequest[eActuator].eStatus = REQUEST_TREATED;
				}
			}
		}
		vTaskDelay(u8TaskPeriod/portTICK_RATE_MS);
	}
	vTaskDelay(portMAX_DELAY);
}


tsActuatorTaskRequest* psActuatorMountRequest(teActuatorId eId, teActuatorTaskRequestType eType)
{
	tsActuatorTaskRequest* psRequest = (tsActuatorTaskRequest*)zalloc(sizeof(tsActuatorTaskRequest));
	psRequest->eId = eId;
	psRequest->eType = eType;
	return psRequest;
}

void vActuatorTaskActivate(teActuatorId eId)
{
	tsMemQueueMessage* psMsg = (tsMemQueueMessage*)zalloc(sizeof(tsMemQueueMessage));
	psMsg->eType = ACTUATOR_MESSAGE;

	tsActuatorTaskRequest* psRequest = psActuatorMountRequest(eId, REQUEST_ACTIVATE);
	psMsg->pvData = (void*) psRequest;
	xQueueSendToBack(xQueue, (void*) psMsg, 0);
	free(psMsg);
}

void vActuatorTaskDeactivate(teActuatorId eId)
{
	tsMemQueueMessage sMsg;
	sMsg.eType = ACTUATOR_MESSAGE;
	tsActuatorTaskRequest* psRequest = psActuatorMountRequest(eId, REQUEST_DEACTIVATE);
	sMsg.pvData = (void*) psRequest;
	xQueueSendToBack(xQueue, (void*) &sMsg, 0);
}

void vActuatorTaskToggleFast(teActuatorId eId, uint8_t u8Duration)
{
	tsMemQueueMessage sMsg;
	sMsg.eType = ACTUATOR_MESSAGE;
	tsActuatorTaskRequest* psRequest;
	if (u8Duration)
	{
		psRequest = psActuatorMountRequest(eId, REQUEST_FLASH_FAST_TIMED);
	}
	else
	{
		psRequest = psActuatorMountRequest(eId, REQUEST_FLASH_FAST_STEADY);
	}
	psRequest->u8Duration = u8Duration;
	sMsg.pvData = (void*) psRequest;
	xQueueSendToBack(xQueue, (void*) &sMsg, 0);
}

void vActuatorTaskToggleSlow(teActuatorId eId, uint8_t u8Duration)
{
	tsMemQueueMessage sMsg;
	sMsg.eType = ACTUATOR_MESSAGE;
	tsActuatorTaskRequest* psRequest;
	if (u8Duration)
	{
		psRequest = psActuatorMountRequest(eId, REQUEST_FLASH_SLOW_TIMED);
	}
	else
	{
		psRequest = psActuatorMountRequest(eId, REQUEST_FLASH_SLOW_STEADY);
	}
	psRequest->u8Duration = u8Duration;
	sMsg.pvData = (void*) psRequest;
	xQueueSendToBack(xQueue, (void*) &sMsg, 0);
}
