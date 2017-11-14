/*
 * event.h
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */

#ifndef CODE_ADAPTER_TYPES_EVENT_H_
#define CODE_ADAPTER_TYPES_EVENT_H_

/**
 * \addtogroup adapter
 * @{
 */

/**
 * \defgroup types Types
 * @brief global type definitions used in all layers of code
 */

/**
 * \addtogroup types
 * @{
 */

/**
 * \defgroup event Events
 * @brief definition of the event enumeration type and related data
 */

/**
 * \addtogroup event
 * @{
 */

/**
 * @brief Event enumeration
 */
typedef enum {
	PIR_SENSOR_TRIGGER,			//<! PIR sensor triggered
	PIR_SENSOR_UNTRIGGER,		//<! PIR sensor not triggered
	N_EVENT_TYPES				//!< N_EVENT_TYPES
} teEventType;


/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif /* CODE_ADAPTER_TYPES_EVENT_H_ */
