/*
 * wifi_task_types.h
 *
 *  Created on: Feb 5, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_WIFI_TASK_WIFI_TASK_TYPES_H_
#define CODE_APPLICATION_WIFI_TASK_WIFI_TASK_TYPES_H_

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

/**
 * @brief Wifi task state
 */
typedef enum {
	WIFI_TASK_IDLE,         //!< Task is idle
	WIFI_TASK_SETUP_STATION,//!< Task is setting up as station
	WIFI_TASK_SETUP_AP      //!< Task is setting up as access point
} teWifiTaskState;

/**
 * @}
 * @}
 */
#endif /* CODE_APPLICATION_WIFI_TASK_WIFI_TASK_TYPES_H_ */
