/*
 *
 * behaviour_task.c
 *
 *  Created on: Feb 25, 2018
 *      Author: Lucas
 */
#include "behavior_task.h"
#include "mem_types.h"
#include "comm_protocol.h"
#include "devices.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

void prv_vBehaviourDoCountdownOnDevices(tsBehaviourTaskConfiguration* psTask);
tsProtocolMessage* prv_psBehaviourGetMessage(tsBehaviourTaskConfiguration* psTask);

xListOfDevices* pxDevList;

void vBehaviorTask(void* pvParemeters)
{
	tsBehaviourTaskConfiguration* psTask = (tsBehaviourTaskConfiguration*) pvParemeters;
	psTask->xLastWakeTime = xTaskGetTickCount();

	vBehaviourDetectMode(psTask);
	tsProtocolMessage* psProtMessage = NULL;
	while(1)
	{
		psProtMessage = prv_psBehaviourGetMessage(psTask);
		switch (psTask->eMode)
		{
		case HEAD:
		{
			psTask->u32DeviceTimeoutCounter += psTask->u32Period;
			if (psTask->u32DeviceTimeoutCounter >= psTask->u32BroadcastPeriod)
			{
				psTask->u32DeviceTimeoutCounter = 0;
				prv_vBehaviourDoCountdownOnDevices(psTask);
			}
			if (psProtMessage == NULL)
			{
				continue;
			}
			switch (psProtMessage->eType)
			{
			case MSG_STATUS:
			{
				tsDevice* psDeviceInfo = (tsDevice*)zalloc(sizeof(tsDevice));
				memcpy((void*)(((uint8_t*)psDeviceInfo) + 4), (void*)((uint8_t*)psProtMessage->acData_), sizeof(tsDevice)-4);
				psDeviceInfo->u8Countdown = 5;
				LOG_DEBUG("Message from %u", psDeviceInfo->u32ID);
				tsDevice* psDeviceInList = psDeviceGetById(&(psTask->xDeviceList), psDeviceInfo->u32ID);
				if (psDeviceInList == NULL)
				{
					// Device is not yet registered, register it in list
					LOG_DEBUG("New device - id:%u", psDeviceInfo->u32ID);
					vDeviceAppend(&psTask->xDeviceList, psDeviceInfo);
					LOG_DEV_LIST(psTask->xDeviceList);
				}
				else
				{
					// Update the device
					LOG_DEBUG("Update device - id:%u", psDeviceInfo->u32ID);
					vDeviceUpdate(psDeviceInList, psDeviceInfo);
					free(psDeviceInfo);
				}
			}
			}
			break;
		}
		case NODE:
		{
			if (psProtMessage == NULL)
			{
				continue;
			}
			if (psProtMessage->eType == MSG_BROADCAST)
			{
				tsProtocolMessage* psStatusMsg = psProtocolMakeStatus(psTask->u8MasterIP_);
				vProtocolSendMessage(psStatusMsg);
				free(psStatusMsg);
			}
			break;
		}

		default:
		{
			if (psProtMessage == NULL)
			{
				continue;
			}
			break;
		}
		}

		free(psProtMessage);

	}

}

void vBehaviourDetectMode(tsBehaviourTaskConfiguration* psTask)
{
	const uint32_t u32MinTimeout = 3000;  // milliseconds
	const uint32_t u32MaxTimeout = 10000; // milliseconds
	uint32_t u32Timeout = (u32SystemRandom() % (u32MaxTimeout - u32MinTimeout)) + u32MinTimeout;
	pxDevList = &psTask->xDeviceList;
	LOG_DEBUG("Will wait for %u ms", u32Timeout);

	// Check whether the wifi task will be able to connect. If fail, the behavior mode sets to disconnected
	teException eWifiException = eWifiWaitToBeConnected(30);
	if (eWifiException != EX_SUCCESSFUL)
	{
		psTask->eMode = NOT_CONNECTED;
		LOG_DEBUG("Set as NOT_CONNECTED");
	}
	else
	{
		vProtocolInit();
		tsMemQueueMessage* psQueueMessage = (tsMemQueueMessage*) zalloc(sizeof(tsMemQueueMessage));
		// Do setup, checking if there is an head in the connected network
		if (xQueueReceive(psTask->xBehaviorQueue, (const void*) psQueueMessage, u32Timeout/portTICK_RATE_MS) == pdTRUE)
		{
			tsProtocolMessage* psProtMessage = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
			vProtocolDecodeUDP((tsUDPMessage*) psQueueMessage->pvData, psProtMessage);
			psTask->eMode = NODE;
			memcpy(psTask->u8MasterIP_, psProtMessage->u8IP_, 4);
			LOG_DEBUG("Set as NODE - master IP: %u . %u . %u . %u ", psTask->u8MasterIP_[0], psTask->u8MasterIP_[1], psTask->u8MasterIP_[2], psTask->u8MasterIP_[3]);
			free(psQueueMessage->pvData);
			free(psProtMessage);
		}
		else
		{
			vProtocolDoBroadcast();
			psTask->eMode = HEAD;
			xTimerStart(psTask->xBroadcastTimer, 0);
			LOG_DEBUG("Set as HEAD");

		}
		free(psQueueMessage);
	}
}

void vBehaviourBroadcastCallback(xTimerHandle pxTimer)
{
	vProtocolDoBroadcast();
}

xListOfDevices pxBehaviourGetListOfDevices()
{
	return *pxDevList;
}

void prv_vBehaviourDoCountdownOnDevices(tsBehaviourTaskConfiguration* psTask)
{
	tsDevice* psDev = (tsDevice*) psTask->xDeviceList;
	while(psDev)
	{
		psDev->u8Countdown -= 1;
		if (psDev->u8Countdown <= 0)
		{
			tsDevice* psNext = (tsDevice*)psDev->pvNext;
			vDeviceRemove(&psTask->xDeviceList, psDev);
			LOG_DEBUG("Device %u is down", psDev->u32ID);
			LOG_DEV_LIST(psTask->xDeviceList);
			free(psDev);
			psDev = psNext;
		}
		else
		{
			psDev = (tsDevice*) psDev->pvNext;
		}
	}
}

tsProtocolMessage* prv_psBehaviourGetMessage(tsBehaviourTaskConfiguration* psTask)
{
	tsProtocolMessage* psProtMsg = NULL;
	tsMemQueueMessage* psQueueMsg = (tsMemQueueMessage*)zalloc(sizeof(tsMemQueueMessage));
	portBASE_TYPE xNewMessage = xQueueReceive(psTask->xBehaviorQueue, psQueueMsg, psTask->u32Period/portTICK_RATE_MS);

	if (xNewMessage == pdTRUE)
	{
		psProtMsg = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
		vProtocolDecodeUDP((tsUDPMessage*)psQueueMsg->pvData, psProtMsg);
		LOG_UDP(psQueueMsg->pvData);
		free(psQueueMsg->pvData);
	}
	free(psQueueMsg);
	return psProtMsg;
}
