/*
 * sensor_types.h
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */

#ifndef CODE_MODULE_SENSOR_SENSOR_TYPES_H_
#define CODE_MODULE_SENSOR_SENSOR_TYPES_H_

#include "gpio_adapter.h"
#include "basictypes.h"
#include "freertos/queue.h"

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

/**
 * \defgroup sensor_types Types
 * @brief Sensor type definitions
 * @addtogroup sensor_types
 * @{
 */



/**
 * @brief Sensor type enumeration
 */
typedef enum {
	PIR,    			//!< Depicts an PIR sensor
	SWITCH, 			//!< Depicts a switch
	INVALID_SENSOR,		//!< Sensor not initialized
	N_SENSOR			//!< Type of sensor count
} teSensorType;


/**
 * @brief Sensor state enumeration
 */
typedef enum {
	UNTRIGGERED,		//!< Sensor is not triggered
	TRIGGERED			//!< Sensor is triggered
} teSensorState;

/**
 * @brief Sensor definition structure
 */
typedef struct {
	teSensorType eType;				//!< Type of the sensor
	teGPIONumber eGPIOAssigned;		//!< GPIO assigned to the sensor
	uint8 u8Id;						//!< Id of the sensor
	uint8 u8IntId;					//!< Interrupt id
	teSensorState eState;			//!< Current state of the sensor
	uint8 __reserved0[3];			//!< Reserved for allignment
} tsSensor;

/**
 * @brief Sensor control structure
 */
typedef struct {
	xQueueHandle xQueueFromISRToTask;	//!< Queue where the GPIO ISR should put it's events
} tsSensorControl;

/**
 * @brief Sensor event data
 */
typedef struct {
	uint8 u8Id;							//!< ID of the sensor
	teSensorState eState;				//!< New state of the sensor
	uint8 __reserved[2];				//!< Allignment
} tsSensorEvent;

typedef struct {
	teSensorState eState;				//!< New state of sensor
	uint32_t u32Id;						//!< ID of the triggered device
} tsNetworkEvent;

/**
 * @}
 * @}
 */
/**
 * @}
 */

#endif /* CODE_MODULE_SENSOR_SENSOR_TYPES_H_ */
