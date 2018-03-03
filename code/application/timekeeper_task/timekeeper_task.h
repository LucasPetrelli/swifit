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

typedef struct {
	uint32_t u32Period;
	portTickType xLastWakeUpTick;
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
