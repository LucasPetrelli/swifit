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

void vTaskWifi(void *pvParameters);

#endif /* CODE_APPLICATION_WIFI_TASK_WIFI_TASK_H_ */
