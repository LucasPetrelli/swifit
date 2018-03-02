/*
 * configuration.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_CONFIGURATION_CONFIGURATION_H_
#define CODE_MODULE_CONFIGURATION_CONFIGURATION_H_

#include "configuration_types.h"

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
 * \defgroup configuration_api API
 * @brief Device running configuration APIs
 * @addtogroup configuration_api
 * @{
 */

/**
 * @brief Reads the device configuration from the file system
 */
void vConfigurationRead();

/**
 * @brief Overrides the device configuration with the default on the file system
 */
void vConfigurationSetDefault();

/**
 * @brief Sets the current configuration
 * @param psConfig pointer to new configuration structure.
 *
 * The argument structure can be freed/deleted after this call.
 */
void vConfigurationSet(tsConfiguration* psConfig);

/**
 * @brief Gets the current configuration
 * @return A pointer to the current configuration
 */
tsConfiguration* psConfigurationGet();

/**
 * @brief Clears the message field from the device configuration
 */
void vConfigurationClearMessage();

/**
 * @brief Gets the device name
 * @return Pointer to the zero terminatted string of the device name
 *
 * Gets the device name either from the RAM or FLASH
 */
char* zConfigurationGetName();

/**
 * @brief Sets the device name
 * @param zName Pointer to the zero terminatted string of the device name
 *
 * Sets the devie name in the RAM and FLASH
 */
void vConfigurationSetName(char* zName);

/**
 * @brief Reads from flash memory the starting actuator state
 * @return The actuator state (0 - DEACTIVATED, 1 - ACTIVATED)
 */
uint8_t u8ConfigurationGetActuatorState();

/**
 * @brief Sets in the flash memory the starting actuator state
 * @param u8State The actuator state (0 - DEACTIVATED, 1 - ACTIVATED)
 */
void vConfigurationSetActuatorState(uint8_t u8State);


/**
 * @}
 * @}
 * @}
 */
#endif /* CODE_MODULE_CONFIGURATION_CONFIGURATION_H_ */
