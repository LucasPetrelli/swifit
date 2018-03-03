/*
 * timing_task.h
 *
 *  Created on: Mar 2, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_TIMEKEEPER_TASK_TIMEKEEPER_TASK_H_
#define CODE_APPLICATION_TIMEKEEPER_TASK_TIMEKEEPER_TASK_H_

/**
 * \defgroup application Application
 * @brief Application tasks that handle the proper feature implementation
 * @addtogroup application
 * @{
 *
 * \defgroup timekeeper Timekeeper
 * @brief Task dedicated to take care of the timekeeping
 * @addtogroup timekeeper
 * @{
 */
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


typedef enum {
	TIMEKEEPER_WIFI_NOT_CONNECTED,
	TIMEKEEPER_NOT_SYNC,
	TIMEKEEPER_SYNC
}teTimekeeperState;

/**
 * @brief Timekeeper task control structure
 */
typedef struct {
	uint32_t u32Period;						//!< Period of the task
	portTickType xLastWakeUpTick;			//!< Last wake up time
	xQueueHandle xTimekeeperQueue;			//!< Queue from which the task receives messages

	teTimekeeperState eState;				//!< Current timekeeper state
} tsTimekeeperTaskConfiguration;

/**
 * @brief Timekeeper task body
 * @param pvParameters
 */
void vTimekeeperTask(void* pvParameters);
/**
 * @}
 * @}
 */

#endif /* CODE_APPLICATION_TIMEKEEPER_TASK_TIMEKEEPER_TASK_H_ */
