/*
 * behaviour_task.h
 *
 *  Created on: Feb 25, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_BEHAVIOR_TASK_BEHAVIOR_TASK_H_
#define CODE_APPLICATION_BEHAVIOR_TASK_BEHAVIOR_TASK_H_

/**
 * \defgroup application Application
 * @brief Application tasks that handle the
 * @addtogroup application
 * @{
 *
 * \defgroup behaviourtask Behaviour
 * @brief Task that receives inputs and take actions based on the current state of execution
 * @addtogroup behaviourtask
 * @{
 */

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "behavior_task_types.h"

/**
 * @brief Behavior task main body
 * @param pvParemeters a tsBehaviourTaskConfiguration structure
 */
void vBehaviorTask(void* pvParemeters);


/**
 * @brief Detects the current operation mode
 *
 * Detects the current operation mode, by first querying the current wifi state, then
 */
void vBehaviourDetectMode(tsBehaviourTaskConfiguration* psTask);

/**
 * @brief callback for the broadcast timer
 *
 * Sends the protocol broadcast message. It's only called when the device is running under master behaviour.
 */
void vBehaviourBroadcastCallback(xTimerHandle pxTimer);

void vBehaviourPutInQueue(tsMemQueueMessage* psMsg);

/**
 * @brief gets the list of known devices
 * @return a pointer to the first object of the list
 */
xListOfDevices pxBehaviourGetListOfDevices();

/**
 * @}
 * @}
 */

#endif /* CODE_APPLICATION_BEHAVIOR_TASK_BEHAVIOR_TASK_H_ */
