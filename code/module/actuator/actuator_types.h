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
 */

/**
 * @addtogroup module
 * @{
 */

/**
 * \defgroup actuator Actuator
 * @brief Handling of actuator elements
 */
/**
 * @addtogroup actuator
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
	DEACTIVATED,
	ACTIVATED
} teActuatorState;

/**
 * @brief Actuator ids
 */
typedef enum {
	LED_RED,
	LED_BLUE,
	MAIN_RELAY,
	N_ACTUATORS
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



#endif /* CODE_MODULE_ACTUATOR_ACTUATOR_TYPES_H_ */
