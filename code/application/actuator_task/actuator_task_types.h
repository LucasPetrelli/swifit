/*
 * actuator_task_types.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_TYPES_H_
#define CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_TYPES_H_
#include "actuator_types.h"


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
 * @brief actuator request type enumeration
 */
typedef enum {
	REQUEST_DEACTIVATE,				//!< Request to deactivate
	REQUEST_ACTIVATE,				//!< Request to activate
	REQUEST_FLASH_FAST_TIMED,		//!< Request to toggle fast during an amount of time
	REQUEST_FLASH_SLOW_TIMED,		//!< Request to toggle slow during an amount of time
	REQUEST_FLASH_FAST_STEADY,		//!< Request to toggle fast
	REQUEST_FLASH_SLOW_STEADY,		//!< Request to toggle slow
} teActuatorTaskRequestType;

/**
 * @brief actuator request status enumeration
 */
typedef enum {
	REQUEST_UNTREATED,      		//!< REQUEST_UNTREATED
	REQUEST_UNDER_TREATMENT,		//!< REQUEST_UNDER_TREATMENT
	REQUEST_TREATED,        		//!< REQUEST_TREATED
} teActuatorTaskRequestStatus;

/**
 * @brief actuator request structure
 */
typedef struct {
	teActuatorTaskRequestType eType;		//!< The type of the request
	teActuatorId eId;						//!< The id of the target actuator
	teActuatorTaskRequestStatus eStatus;	//!< The status of the request
	uint8_t u8Duration;						//!< The duration of the request
	uint32_t u32Id;							//!< Target identity
} tsActuatorTaskRequest;

/**
 * @}
 * @}
 */
#endif /* CODE_APPLICATION_ACTUATOR_TASK_ACTUATOR_TASK_TYPES_H_ */
