/**
 * @file wifi_task.c
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */
#include "wifi_task.h"
#include "configuration.h"
#include "system_adapter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mem_types.h"

void vTaskWifi(void *pvParameters)
{
	tsConfiguration* psCurrentConfiguration = psConfigurationGet();
	tsWifiTaskConfiguration* psTask = (tsWifiTaskConfiguration*) pvParameters;
	vWifiSetNotificationQueue(psTask->xWifiNotificationQueue);


	// Prepare initial state based on the last saved configuration
	switch (psCurrentConfiguration->eMode)
	{
		//Device in configuration mode, set up as access point
		case CONFIG_MODE:
		{
			psTask->eState = WIFI_TASK_SETUP_AP;
			break;
		}

		// Device was connected to a valid access point, setup as station
		// and mark as trying to connect to known AP
		case NORMAL_MODE:
		{
			psCurrentConfiguration->eMode = CONNECTING_TO_KNOWN_AP;
			vConfigurationSet(psCurrentConfiguration);
			psTask->eState = WIFI_TASK_SETUP_STATION;
			break;
		}

		// Device will try to connect to the new AP given in last CONFIG_MODE
		// It is saved in the psCurrentConfiguration
		case ATTEMPT_NEW_CONNECTION:
		{
			psTask->eState = WIFI_TASK_SETUP_STATION;
			break;
		}

		// Device was shut down while trying to connect to a known AP
		// Keep in this state while the AP is not reached
		case CONNECTING_TO_KNOWN_AP:
		{
			psTask->eState = WIFI_TASK_SETUP_STATION;
			break;
		}
	}

	//vWifiInitMDNS();

	while(1)
	{
		switch (psTask->eState)
		{
			// Setup as access point - will create it's own network
			case WIFI_TASK_SETUP_AP:
			{
				LOG_DEBUG("CONFIG MODE - setting as access point");
				vWifiSetMode(WIFI_AP);
				vWifiStartAP();
				psTask->eState = WIFI_TASK_IDLE;
				break;
			}

			// Setup as station
			// Try to connect to the AP available in the configuration structure
			// If failed, act accordingly to the current state in the device configuration
			case WIFI_TASK_SETUP_STATION:
			{

				LOG_DEBUG("OP MODE - establishing connection");
				vWifiSetMode(WIFI_STATION);
				vWifiSetHostname(zConfigurationGetName());

				teException eException;
				// Try to connect to the access point
				// This call is asynchronous, returning failure if something goes wrong in the launch process
				eException = eWifiConnectToAP(psCurrentConfiguration->acAP_, psCurrentConfiguration->acPassword_);
				if (eException != EX_SUCCESSFUL)
				{
					LOG_DEBUG("Something went wrong in connection setup");
					psTask->eState = WIFI_TASK_SETUP_AP;
					break;
				}


				LOG_DEBUG("Connection setup OK, waiting for connection");
				uint8_t u8Timeout = 20;								// Timeout in seconds
				eException = eWifiWaitToBeConnected(u8Timeout);

				// Peek eException for result
				if (eException != EX_SUCCESSFUL)
				{
					// If the AP was reached before, retry
					if (psCurrentConfiguration->eMode == CONNECTING_TO_KNOWN_AP)
					{
						continue;
					}

					// If not, will setup a config mode to retrieve a new AP data from the user
					psCurrentConfiguration->eMode = CONFIG_MODE;

					// Set a message to be displayed in the config page
					LOG_DEBUG("Something went wrong WHILE trying to connect, going to AP mode");

					memset((void*)psCurrentConfiguration->acMessage_, 0, strlen(psCurrentConfiguration->acMessage_));
					switch (eException)
					{
					case EX_WIFI_AP_NOT_FOUND:
					{
						strcat(psCurrentConfiguration->acMessage_, psCurrentConfiguration->acAP_);
						strcat(psCurrentConfiguration->acMessage_, " was not found!");
						break;
					}
					case EX_WIFI_WRONG_PASSWORD:
					{
						strcat(psCurrentConfiguration->acMessage_, "Password invalid for ");
						strcat(psCurrentConfiguration->acMessage_, psCurrentConfiguration->acAP_);
						break;
					}
					default:
					{
						strcat(psCurrentConfiguration->acMessage_, "Failed to connect to ");
						strcat(psCurrentConfiguration->acMessage_, psCurrentConfiguration->acAP_);
					}
					}
					vConfigurationSet(psCurrentConfiguration);

					// Call a system reset
					vSystemReset();

					// This code won't be reached, but keep it here for information
					psTask->eState = WIFI_TASK_SETUP_AP;
					break;
				}

				// If successful, config device as NORMAL MODE
				psCurrentConfiguration->eMode = NORMAL_MODE;
				memset((void*)psCurrentConfiguration->acMessage_, 0, strlen(psCurrentConfiguration->acMessage_));
				vConfigurationSet(psCurrentConfiguration);

				// Wifi task is IDLE
				psTask->eState = WIFI_TASK_IDLE;
				break;
			}
			case WIFI_TASK_IDLE:
			{
				tsMemQueueMessage sMsg;
				portBASE_TYPE xResult;

				// Blocks until a notification is received from the wifi ISR
				xResult = xQueueReceive(psTask->xWifiNotificationQueue, (void*)&sMsg, portMAX_DELAY);

				if (xResult == pdTRUE)
				{
					teWifiConnectionStatus eNewStatus = (teWifiConnectionStatus) sMsg.pvData;
					LOG_DEBUG("Wifi task received %u", eNewStatus);

					// Notify other tasks
					tsMemQueueMessage sNotification;
					sNotification.eType = WIFI_MESSAGE;
					sNotification.pvData = (void*) eNewStatus;
					portBASE_TYPE xSendResult;
					if (psTask->xBehaviorNotificationQueue)
					{
						LOG_DEBUG("Notifying behavior");
						xSendResult = xQueueSend(psTask->xBehaviorNotificationQueue, (void*)&sNotification, 0);
					}
					if (psTask->xTimekeeperNotificationQueue)
					{
						LOG_DEBUG("Notifying timekeeper");
						xSendResult = xQueueSend(psTask->xTimekeeperNotificationQueue, (void*)&sNotification, 0);
					}
				}
				break;
			}
			default:
				psTask->eState = WIFI_TASK_IDLE;
				break;
		}
	}
}

