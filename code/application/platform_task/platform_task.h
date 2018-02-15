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

typedef struct {
	portTickType xLastWakeTime;
	uint32_t u32Period;

	uint32_t u32HeapReportCounter;
	uint32_t u32HeapReportPeriod;

} tsPlatformTaskConfiguration;

void vPlatformTask(void* pvParameters);
void vScheduleReset(uint32_t u32MillisDelay);
void vPlatformSetLEDs(teOperationMode eMode);


#endif /* CODE_APPLICATION_PLATFORM_TASK_PLATFORM_TASK_H_ */
