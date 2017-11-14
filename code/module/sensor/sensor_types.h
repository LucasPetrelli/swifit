/*
 * sensor_types.h
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */

#ifndef CODE_MODULE_SENSOR_SENSOR_TYPES_H_
#define CODE_MODULE_SENSOR_SENSOR_TYPES_H_

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 */
/**
 * @addtogroup module
 * @{
 */

/**
 * \defgroup sensor Sensor
 * @brief Handling of sensor elements
 */
/**
 * @addtogroup sensor
 * @{
 */

#include "gpio_adapter.h"
#include "basictypes.h"
#include "freertos/queue.h"


/**
 * @brief Sensor type enumeration
 */
typedef enum {
	UNASSIGNED,		//!< Sensor not initialized
	PIR,    		//!< Depicts an PIR sensor
	SWITCH, 		//!< Depicts a switch
	N_SENSOR		//!< Type of sensor count
} teSensorType;


/**
 * @brief Sensor state depiction
 */
typedef enum {
	DEACTIVATED,
	ACTIVATED
} teSensorState;

/**
 * @brief Sensor definition structure
 */
typedef struct {
	teSensorType eType;				//!< Type of the sensor
	teGPIONumber eGPIOAssigned;		//!< GPIO assigned to the sensor
	uint8 u8Id;						//!< Id of the sensor
	uint8 u8IntId;					//!< Interrupt id
	teSensorState eState;
	uint8 __reserved0[3];
} tsSensor;

/**
 * @brief Sensor control structure
 */
typedef struct {
	xQueueHandle xQueueFromISRToTask;
} tsSensorControl;

/**
 * @brief Sensor event data
 */
typedef struct {
	uint8 u8Id;
	teSensorState eState;
	uint8 __reserved[2];
} tsSensorEvent;

/**
 * @}
 */
/**
 * @}
 */

#endif /* CODE_MODULE_SENSOR_SENSOR_TYPES_H_ */
