/*
 * devices_types.h
 *
 *  Created on: Feb 26, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_DEVICES_DEVICES_TYPES_H_
#define CODE_MODULE_DEVICES_DEVICES_TYPES_H_

#include "actuator_types.h"
#include "sensor_types.h"
#include "basictypes.h"

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 *
 * \defgroup devices Devices descriptors
 * @brief Handling and registry of the networks devices information
 * @{
 *
 * \defgroup devices_types Types
 * @brief Devices descriptors type definitions
 * @addtogroup devices_types
 * @{
 */

/**
 * @brief Device type enumeration
 */
typedef enum {
	DEVICE_SOCKET, 	//!< The device is a wall socket
	DEVICE_SWITCH	//!< The device is a wall switch
} teDeviceType;

/**
 * @brief Device information structure
 */
typedef struct {
	void* pvNext;					//!< Pointer to be used by list functionality

	uint8_t u8IP_[4];				//!< The device IP
	uint32_t u32ID;					//!< The device ID

	char cName_[32];				//!< The device name given by the user

	teDeviceType eType;				//!< The device type
	teActuatorState eActuatorState;	//!< Device actuator state (relay)
	teSensorState eSensorState;		//!< Device sensor state (may be a PIR or a switch)

	uint8_t u8Countdown;

} tsDevice;

/**
 * @brief Mask definition for a list of devices
 */
typedef tsDevice* xListOfDevices;

#endif /* CODE_MODULE_DEVICES_DEVICES_TYPES_H_ */
