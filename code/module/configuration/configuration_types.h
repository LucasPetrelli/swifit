/*
 * configuration_types.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_CONFIGURATION_CONFIGURATION_TYPES_H_
#define CODE_MODULE_CONFIGURATION_CONFIGURATION_TYPES_H_

#include "basictypes.h"

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 *
 * \defgroup configuration Configuration
 * @brief Handling of the device running configuration
 * @{
 *
 * \defgroup configuration_types Types
 * @brief Device running configuration type definitions
 * @addtogroup configuration_types
 * @{
 */

#define NAME_LENGTH ((uint32_t) 32)

/**
 * @brief Device operation type
 *
 * This enumeration follows the ASCII pattern instead of a integer counter
 */
typedef enum {
	CONFIG_MODE = '0',				/**< Device in configuration mode */
	NORMAL_MODE = '1',				/**< Device in normal operation mode */
	ATTEMPT_NEW_CONNECTION = '2',	/**< Device trying to connect to a new AP given in CONFIG_MODE */
	CONNECTING_TO_KNOWN_AP = '3',   /**< Device trying to connect to an AP previously verified */
} teOperationMode;

/**
 * @brief Device configuration structure
 */
typedef struct {
	char acAP_[32];					//!< The device access point name
	char acPassword_[32];			//!< The access point password
	char acMessage_[64];			//!< A message saved in the memory
	teOperationMode eMode;			//!< The current operation mode
} tsConfiguration;

/**
 * @}
 * @}
 * @}
 */

#endif /* CODE_MODULE_CONFIGURATION_CONFIGURATION_TYPES_H_ */
