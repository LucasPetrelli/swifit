/*
 * actuator_task.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_H_
#define CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_H_

#include "actuator.h"
#include "actuator_task_types.h"

void vActuatorTask(void* pvParameters);
void vActuatorTaskDeactivate(teActuatorId eId);
void vActuatorTaskActivate(teActuatorId eId);
void vActuatorTaskToggleFast(teActuatorId eId, uint8_t u8Duration);

#endif /* CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_H_ */
