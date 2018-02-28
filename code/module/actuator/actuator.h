/*
 * actuator.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_ACTUATOR_ACTUATOR_H_
#define CODE_MODULE_ACTUATOR_ACTUATOR_H_

// Type definition
#include "actuator_types.h"



/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 *
 * \defgroup actuator Actuator
 * @brief Handling of actuator elements
 * @{
 *
 * \defgroup actuator_api API
 * @brief Actuator handling functions
 * @addtogroup actuator_api
 * @{
 */

/**
 * @brief Maximum number of actuators used in the platform
 */
#define MAX_ACTUATOR 3


/**
 * @brief Initialization of the platform peripherals related to the actuators
 *
 * Configures the actuator related GPIOs and initializes their control structures
 */
void vActuatorInitPlatform();

/**
 * @brief Activates an actuator
 * @param psActuator pointer to the actuator control structure
 *
 * Activates an actuator described by the psActuator structure, refreshing the structure
 */
void vActuatorActivate(tsActuator* psActuator);

/**
 * @brief Deactivates an actuator
 * @param psActuator pointer to the actuator control structure
 *
 * Deactivates an actuator described by the pasActuator structure, refreshing the structure
 */
void vActuatorDeactivate(tsActuator* psActuator);

/**
 * @brief Activates an actuator
 * @param eId unique identity of the actuator
 *
 * Activates the actuator related to eId
 */
void vActuatorActivateById(teActuatorId eId);

/**
 * @brief Deactivates an actuator
 * @param eId unique identity of the actuator
 *
 * Deactivates the actuator related to eId
 */
void vActuatorDeactivateById(teActuatorId eId);

/**
 * @brief Toggle the state of an actuator
 * @param eId unique identity of the actuator
 *
 * Toggles the state of the actuator related to eId
 */
void vActuatorToggleById(teActuatorId eId);

/**
 * @brief Gets the current state of an actuator
 * @param eId identity of the actuator
 * @return the state of the actuator
 */
teActuatorState eActuatorGetStateById(teActuatorId eId);

/**
 * @}
 * @}
 * @}
 */


#endif /* CODE_MODULE_ACTUATOR_ACTUATOR_H_ */
