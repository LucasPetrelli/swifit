/*
 * platform_task.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_PLATFORM_TASK_PLATFORM_TASK_H_
#define CODE_APPLICATION_PLATFORM_TASK_PLATFORM_TASK_H_

#include "basictypes.h"
#include "configuration.h"

#include "actuator.h"
/**
 * \defgroup application Application
 * @brief Application tasks that handle the
 * @addtogroup application
 * @{
 *
 * \defgroup platformtask Platform
 * @brief Task that oversees the current state of the platform
 * @addtogroup platformtask
 * @{
 */

/**
 * @brief Platform task configuration structure
 */
typedef struct {
	portTickType xLastWakeTime;				//!< Timestamp (in ticks) of the last wake up
	uint32_t u32Period;						//!< Task period (in milliseconds)

	uint32_t u32HeapReportCounter;			//!< Counter that when overflow a heap report should occur
	uint32_t u32HeapReportPeriod;			//!< Value on which the u32HeapReportCounter should overflow

} tsPlatformTaskConfiguration;

/**
 * @brief Platform task main body
 * @param pvParameters a tsPlatformTaskConfiguration structure
 */
void vPlatformTask(void* pvParameters);

/**
 * @brief Schedules a system reset
 * @param u32MillisDelay The time to wait before the reset
 */
void vScheduleReset(uint32_t u32MillisDelay);

/**
 * @brief Sets the platform LEDs accordingly to the current mode
 * @param eMode the current operation mode
 */
void vPlatformSetLEDs(teOperationMode eMode);

/**
 * @brief Sets the platform red LED accordingly to the current relay state
 * @param eState current relay state
 */
void vPlatformSetRelayLED(teActuatorState eState);

/**
 * @}
 * @}
 */

#endif /* CODE_APPLICATION_PLATFORM_TASK_PLATFORM_TASK_H_ */
