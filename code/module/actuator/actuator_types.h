/*
 * actuator_types.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_ACTUATOR_ACTUATOR_TYPES_H_
#define CODE_MODULE_ACTUATOR_ACTUATOR_TYPES_H_

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
 * \defgroup actuator_types Types
 * @brief Actuator type definitions
 * @addtogroup actuator_types
 * @{
 */

#include "gpio_adapter.h"
#include "basictypes.h"


/**
 * @brief Actuator type enumeration
 */
typedef enum {
	INVALID_ACTUATOR,	//!< Actuator not initialized
	LED,				//!< Depicts an LED
	RELAY				//!< Depicts an Relay
} teActuatorType;

/**
 * @brief Actuator state
 */
typedef enum {
	DEACTIVATED,		//!< Actuator is not activated
	ACTIVATED			//!< Actuator is activated
} teActuatorState;

/**
 * @brief Actuator ids
 */
typedef enum {
	LED_RED,			//!< The platform red LED, used to depict the relay state
	LED_BLUE,			//!< The platform blue LED, used to depict the connection state
	MAIN_RELAY,			//!< The platform relay, used to switch the mains power to the load
	N_ACTUATORS			//!< Depicts the number of actuators avaiable
} teActuatorId;

/**
 * @brief Sensor definition structure
 */
typedef struct {
	teActuatorType eType;			//!< Type of the actuator
	teGPIONumber eGPIOAssigned;		//!< GPIO assigned to the actuator
	teActuatorId eId;				//!< Id of the actuator
	teActuatorState eState;			//!< Current state of the actuator
} tsActuator;

/**
 * @}
 * @}
 * @}
 */

#endif /* CODE_MODULE_ACTUATOR_ACTUATOR_TYPES_H_ */
