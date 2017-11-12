/**
 * @file wifi_task.c
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */
#include "wifi_task.h"

void vTaskWifi(void *pvParameters)
{
	vWifiSetMode(WIFI_STATION);
	eWifiConnectToAP("Flink-22A4", "Falp24gh");
	eWifiWaitToBeConnected();
	vWifiInitMDNS();
//    vWifiStartAP();
	vTaskDelay(portMAX_DELAY);
}

