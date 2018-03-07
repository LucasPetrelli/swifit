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
#include "configuration_types.h"
#include "timing_types.h"

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

#define N_TIME_ENTRIES 4
#define N_RULE_ENTRIES 16

/**
 * @brief Device type enumeration
 */
typedef enum {
	DEVICE_SOCKET, 	//!< The device is a wall socket
	DEVICE_SWITCH	//!< The device is a wall switch
} teDeviceType;

/**
 * @brief Network rule status
 */
typedef enum {
	RULE_DISABLED,	//!< RULE_DISABLED
	RULE_ENABLED  	//!< RULE_ENABLED
} teRuleState;

/**
 * @brief Network rule
 */
typedef struct {
	uint32_t u32TriggerId;				//!< The device ID that the trigger should come from
	uint32_t u32TargetId;				//!< The device ID that should change
	teSensorState eTriggerState;		//!< The trigger level
	teActuatorState eTargetState;		//!< The change level
	teRuleState eEnabled;				//!< If the rule should be taken into account or not
} tsNetworkRule;


/**
 * @brief Device information structure
 */
typedef struct {
	void* pvNext;					//!< Pointer to be used by list functionality

	uint8_t u8IP_[4];				//!< The device IP
	uint32_t u32ID;					//!< The device ID

	char cName_[NAME_LENGTH];		//!< The device name given by the user

	teDeviceType eType;				//!< The device type
	teActuatorState eActuatorState;	//!< Device actuator state (relay)
	teSensorState eSensorState;		//!< Device sensor state (may be a PIR or a switch)

	uint8_t u8Countdown;

	tsTimingEntry sTimeTable_[N_TIME_ENTRIES];
} tsDevice;

typedef struct {
	uint32_t u32Id;
	tsTimingEntry sTable_[N_TIME_ENTRIES];
}tsDeviceTimingChange;


/**
 * @brief Mask definition for a list of devices
 */
typedef tsDevice* xListOfDevices;

#endif /* CODE_MODULE_DEVICES_DEVICES_TYPES_H_ */
