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

void vTaskWifi(void *pvParameters)
{
	tsConfiguration* psCurrentConfiguration = psConfigurationGet();
	teWifiTaskState eTaskState;


	// Prepare initial state based on the last saved configuration
	switch (psCurrentConfiguration->eMode)
	{
		//Device in configuration mode, set up as access point
		case CONFIG_MODE:
		{
			eTaskState = WIFI_TASK_SETUP_AP;
			break;
		}

		// Device was connected to a valid access point, setup as station
		// and mark as trying to connect to known AP
		case NORMAL_MODE:
		{
			psCurrentConfiguration->eMode = CONNECTING_TO_KNOWN_AP;
			vConfigurationSet(psCurrentConfiguration);
			eTaskState = WIFI_TASK_SETUP_STATION;
			break;
		}

		// Device will try to connect to the new AP given in last CONFIG_MODE
		// It is saved in the psCurrentConfiguration
		case ATTEMPT_NEW_CONNECTION:
		{
			eTaskState = WIFI_TASK_SETUP_STATION;
			break;
		}

		// Device was shut down while trying to connect to a known AP
		// Keep in this state while the AP is not reached
		case CONNECTING_TO_KNOWN_AP:
		{
			eTaskState = WIFI_TASK_SETUP_STATION;
			break;
		}
	}

	//vWifiInitMDNS();

	while(1)
	{
		switch (eTaskState)
		{
			// Setup as access point - will create it's own network
			case WIFI_TASK_SETUP_AP:
			{
				LOG_DEBUG("CONFIG MODE - setting as access point");
				vWifiSetMode(WIFI_AP);
				vWifiStartAP();
				eTaskState = WIFI_TASK_IDLE;
				break;
			}

			// Setup as station
			// Try to connect to the AP available in the configuration structure
			// If failed, act accordingly to the current state in the device configuration
			case WIFI_TASK_SETUP_STATION:
			{

				LOG_DEBUG("OP MODE - establishing connection");
				vWifiSetMode(WIFI_STATION);

				teException eException;
				// Try to connect to the access point
				// This call is asynchronous, returning failure if something goes wrong in the launch process
				eException = eWifiConnectToAP(psCurrentConfiguration->acAP_, psCurrentConfiguration->acPassword_);
				if (eException != EX_SUCCESSFUL)
				{
					LOG_DEBUG("Something went wrong in connection setup");
					eTaskState = WIFI_TASK_SETUP_AP;
					break;
				}


				LOG_DEBUG("Connection setup OK, waiting for connection");
				uint8_t u8Timeout = 20;								// Timeout in seconds
				eException = eWifiWaitToBeConnected(u8Timeout);

				// Peek eException for result
				if (eException != EX_SUCCESSFUL)
				{
					// If the AP was reached before, retry
//					if (psCurrentConfiguration->eMode == CONNECTING_TO_KNOWN_AP)
//					{
//						continue;
//					}

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
					eTaskState = WIFI_TASK_SETUP_AP;
					break;
				}

				// If successful, config device as NORMAL MODE
				psCurrentConfiguration->eMode = NORMAL_MODE;
				memset((void*)psCurrentConfiguration->acMessage_, 0, strlen(psCurrentConfiguration->acMessage_));
				vConfigurationSet(psCurrentConfiguration);

				// Wifi task is IDLE
				eTaskState = WIFI_TASK_IDLE;
				break;
			}
			case WIFI_TASK_IDLE:
			{
				vTaskDelay(100/portTICK_RATE_MS);
				break;
			}
			default:
				eTaskState = WIFI_TASK_IDLE;
				break;
		}
	}
}

