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
#include "wifi_types.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

void prv_vBehaviourDoCountdownOnDevices(tsBehaviourTaskConfiguration* psTask);
void prv_vBehaviourHandleActuatorReq(tsBehaviourTaskConfiguration* psTask, tsActuatorTaskRequest* psReq);
void prv_vBehaviorHandleTimingReq(tsBehaviourTaskConfiguration* psTask, tsDeviceTimingChange* psReq);

tsBehaviourTaskConfiguration* prv_psTaskConfig;

void vBehaviorTask(void* pvParemeters)
{
	tsBehaviourTaskConfiguration* psTask = (tsBehaviourTaskConfiguration*) pvParemeters;
	psTask->xLastWakeTime = xTaskGetTickCount();
	prv_psTaskConfig = psTask;
	psTask->eMode = NOT_CONNECTED;
	tsProtocolMessage* psProtMessage = NULL;
	while(1)
	{
		// Retrieve message from queue
		tsMemQueueMessage* psQueueMsg = (tsMemQueueMessage*)zalloc(sizeof(tsMemQueueMessage));
		portBASE_TYPE xNewMessage = xQueueReceive(psTask->xBehaviorQueue, psQueueMsg, psTask->u32Period/portTICK_RATE_MS);

		// If There is a new message, process it
		if (xNewMessage == pdTRUE)
		{
			LOG_DEBUG("Got message");
			switch (psQueueMsg->eType)
			{
			case UDP_MESSAGE_RX:
			{
				psProtMessage = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
				vProtocolDecodeUDP((tsUDPMessage*)psQueueMsg->pvData, psProtMessage);
				LOG_UDP(psQueueMsg->pvData);
				free(psQueueMsg->pvData);
				break;
			}
			case ACTUATOR_MESSAGE:
			{
				// handle message
				tsActuatorTaskRequest* psReq = (tsActuatorTaskRequest*) psQueueMsg->pvData;
				prv_vBehaviourHandleActuatorReq(psTask, psReq);
				break;
			}
			case WIFI_MESSAGE:
			{
				teWifiConnectionStatus eNewWifiStatus = (teWifiConnectionStatus) psQueueMsg->pvData;
				if (eNewWifiStatus == WIFI_CONNECTED_TO_AP)
				{
					// Got connection, detect new mode
					vBehaviourDetectMode(psTask);
					continue;
				}
				else if (eNewWifiStatus == WIFI_DISCONNECTED)
				{
					// Lost connection, set back to disconnected
					psTask->eMode = NOT_CONNECTED;
				}
				break;
			}
			case TIMING_PARAMETER:
			{
				tsDeviceTimingChange* psReq = (tsDeviceTimingChange*) psQueueMsg->pvData;
				prv_vBehaviorHandleTimingReq(psTask, psReq);
				break;
			}
			}
		}
		free(psQueueMsg);

		// Do processing on the protocol message received according to the mode
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
					psTask->u32DeviceTimeoutCounter += psTask->u32Period;
					if (psTask->u32DeviceTimeoutCounter >= 4*psTask->u32BroadcastPeriod)
					{
						psTask->u32DeviceTimeoutCounter = 0 ;
						vBehaviourDetectMode(psTask);
					}
					continue;
				}
				psTask->u32DeviceTimeoutCounter = 0;
				switch (psProtMessage->eType)
				{
					case MSG_BROADCAST:
					{
						tsProtocolMessage* psStatusMsg = psProtocolMakeStatus(psTask->u8MasterIP_);
						vProtocolSendMessage(psStatusMsg);
						free(psStatusMsg);
						break;
					}
					case MSG_ACTION:
					{
						tsActuatorTaskRequest* psReq = (tsActuatorTaskRequest*)zalloc(sizeof(tsActuatorTaskRequest));
						memcpy(psReq ,psProtMessage->acData_, psProtMessage->u32DataCount);
						prv_vBehaviourHandleActuatorReq(psTask, psReq);
						break;
					}
					case MSG_TIMING_PARAMETER:
					{
						tsDeviceTimingChange* psReq = (tsDeviceTimingChange*)zalloc(sizeof(tsDeviceTimingChange));
						memcpy((void*)psReq, (void*)psProtMessage->acData_, psProtMessage->u32DataCount);
						prv_vBehaviorHandleTimingReq(psTask, psReq);
						break;
					}
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
		psProtMessage = NULL;
	}

}

void vBehaviourDetectMode(tsBehaviourTaskConfiguration* psTask)
{
	const uint32_t u32MinTimeout = 3000;  // milliseconds
	const uint32_t u32MaxTimeout = 10000; // milliseconds
	uint32_t u32Timeout = (u32SystemRandom() % (u32MaxTimeout - u32MinTimeout)) + u32MinTimeout;
	LOG_DEBUG("Will wait for %u ms", u32Timeout);
	// Check whether the wifi task will be able to connect. If fail, the behavior mode sets to disconnected
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

void vBehaviourBroadcastCallback(xTimerHandle pxTimer)
{
	vProtocolDoBroadcast();
}

xListOfDevices pxBehaviourGetListOfDevices()
{
	return prv_psTaskConfig->xDeviceList;
}

void vBehaviourPutInQueue(tsMemQueueMessage* psMsg)
{
	xQueueSend(prv_psTaskConfig->xBehaviorQueue, psMsg, 0);
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

void prv_vBehaviourHandleActuatorReq(tsBehaviourTaskConfiguration* psTask, tsActuatorTaskRequest* psReq)
{
	uint32_t u32SelfId = u32SystemGetId();
	if (psReq->u32Id == u32SelfId)
	{
		vActuatorTaskPutInQUeue(psReq);
	}
	else
	{
		tsDevice* psTargetDev = psDeviceGetById(&psTask->xDeviceList, psReq->u32Id);
		if (psTargetDev)
		{
			tsProtocolMessage* psMsg = psProtocolMakeAction(psReq, psTargetDev->u8IP_);
			vProtocolSendMessage(psMsg);
			free(psMsg);
		}
		free(psReq);
	}
}

void prv_vBehaviorHandleTimingReq(tsBehaviourTaskConfiguration* psTask, tsDeviceTimingChange* psReq)
{
	uint32_t u32SelfId = u32SystemGetId();
	if (psReq->u32Id == u32SelfId)
	{
		vConfigurationSetTimeTable(psReq->sTable_);
	}
	else
	{
		tsDevice* psTargetDev = psDeviceGetById(&psTask->xDeviceList, psReq->u32Id);
		if (psTargetDev)
		{
			tsProtocolMessage* psMsg = psProtocolMakeTimingParameter(psReq, psTargetDev->u8IP_);
			vProtocolSendMessage(psMsg);
			free(psMsg);
		}
	}
	free(psReq);
}
