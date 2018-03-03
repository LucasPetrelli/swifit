/**
 * @file wifi_task.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief Wifi task
 */

#ifndef CODE_APPLICATION_WIFI_TASK_WIFI_TASK_H_
#define CODE_APPLICATION_WIFI_TASK_WIFI_TASK_H_
#include "wifi_adapter.h"
#include "wifi_task_types.h"
#include "freertos/FreeRTOS.h"

/**
 * \defgroup application Application
 * @brief Application tasks that handle the proper feature implementation
 * @addtogroup application
 * @{
 *
 * \defgroup wifi Wifi
 * @brief Task dedicated to take care of the Wi-Fi connection
 * @addtogroup wifi
 * @{
 */

typedef struct {
	xQueueHandle xWifiNotificationQueue;
	xQueueHandle xBehaviorNotificationQueue;
	xQueueHandle xTimekeeperNotificationQueue;

	teWifiTaskState eState;
} tsWifiTaskConfiguration;


void vTaskWifi(void *pvParameters);

/**
 * @}
 * @}
 */

#endif /* CODE_APPLICATION_WIFI_TASK_WIFI_TASK_H_ */
