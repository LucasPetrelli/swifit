/*
 * timing_types.h
 *
 *  Created on: Mar 3, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_TIMING_TIMING_TYPES_H_
#define CODE_MODULE_TIMING_TIMING_TYPES_H_

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 * \defgroup timing Timing
 * @brief Handling of timing elements
 * @addtogroup timing
 * @{
 * \defgroup timing_types Types
 * @brief Timing type definitions
 * @addtogroup timing_types
 * @{
 */

#include "u_time.h"
/**
 * @brief Timing action enumeration
 *
 * Enumerates an action that should occur when a time is met
 */
typedef enum {
	TIMING_DEACTIVATE,			//!< Should deactivate the load when the time is met
	TIMING_ACTIVATE   			//!< Should activate the load when the time is met
} teTimingAction;

/**
 * @brief Entry status enumeration
 *
 * Enumerates if an entry is valid and should be taken into account or not
 */
typedef enum {
	TIMING_DISABLED,			//!< The entry is disabled and should be ignored
	TIMING_ENABLED  			//!< The entry is enabled and should be taken into account
} teTimingEntryStatus;

/**
 * @brief Representation of a timing entry in a table(array)
 */
typedef struct {
	teTimingEntryStatus eEnabled;	//!< The entry is valid or not
	teTimingAction eAction;			//!< The action that should occur when the time is met
	uint16_t u16Minutes;			//!< The minute count of the day when the action should occur
} tsTimingEntry;


typedef struct tmElements tsTime;
/**
 * @}
 * @}
 * @}
 */


#endif /* CODE_MODULE_TIMING_TIMING_TYPES_H_ */
