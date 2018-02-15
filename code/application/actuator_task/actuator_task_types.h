/*
 * actuator_task_types.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_TYPES_H_
#define CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_TYPES_H_
#include "actuator_types.h"
typedef enum {
	REQUEST_DEACTIVATE,
	REQUEST_ACTIVATE,
	REQUEST_FLASH_FAST_TIMED,
	REQUEST_FLASH_SLOW_TIMED,
	REQUEST_FLASH_FAST_STEADY,
	REQUEST_FLASH_SLOW_STEADY,
} teActuatorTaskRequestType;

typedef enum {
	REQUEST_UNTREATED,
	REQUEST_UNDER_TREATMENT,
	REQUEST_TREATED,
} teActuatorTaskRequestStatus;

typedef struct {
	teActuatorTaskRequestType eType;
	teActuatorId eId;
	teActuatorTaskRequestStatus eStatus;
	uint8 u8Duration;
} tsActuatorTaskRequest;

#endif /* CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_TYPES_H_ */
