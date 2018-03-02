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

/**
 * \defgroup application Application
 * @brief Application tasks that handle the proper feature implementation
 * @addtogroup application
 * @{
 *
 * \defgroup actuatortask Actuator
 * @brief Task that handles the status of the onboard actuators
 * @addtogroup actuatortask
 * @{
 */

/**
 * @brief Actuator task main body
 * @param pvParameters not used (pass NULL)
 *
 * Handles requests from other tasks to change the state of the actuators
 */
void vActuatorTask(void* pvParameters);

/**
 * @brief Create and send an activate request to the actuator task
 * @param eId The id of the actuator that should be activated
 */
void vActuatorTaskDeactivate(teActuatorId eId);

/**
 * @brief Create and send a deactivate request to the actuator task
 * @param eId The id of the actuator that should be deactivated
 */
void vActuatorTaskActivate(teActuatorId eId);

/**
 * @brief Create and send a toggle fast request to the actuator task
 * @param eId The id of the actuator that should be toggled with a fast frequency
 * @param u8Duration The period of time during which the actuator should be toggled. Send 0 for a steady state toggle.
 *
 * The actuator will be toggled with a 100ms period
 */
void vActuatorTaskToggleFast(teActuatorId eId, uint8_t u8Duration);

/**
 * @brief Create and send a toggle slow request to the actuator task
 * @param eId The id of the actuator that should be toggled with a slow frequency
 * @param u8Duration The period of time during which the actuator should be toggled. Send 0 for a steady state toggle.
 *
 * The actuator will be toggled with a 1s period
 */
void vActuatorTaskToggleSlow(teActuatorId eId, uint8_t u8Duration);


/**
 * @brief Mounts an request structure to be received by an actuator task
 * @param eId ID of the target actuator
 * @param eType Type of the request
 * @return A pointer to the mounter request
 */
tsActuatorTaskRequest* psActuatorMountRequest(teActuatorId eId, teActuatorTaskRequestType eType);

/**
 * @brief Puts a requisition on the task queue
 * @param psReq The requisition to be put in the queue
 */
void vActuatorTaskPutInQUeue(tsActuatorTaskRequest* psReq);

/**
 * @}
 * @}
 */
#endif /* CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_H_ */
