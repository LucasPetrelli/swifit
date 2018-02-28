/*
 * sensor.h
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */

#ifndef CODE_MODULE_SENSOR_SENSOR_H_
#define CODE_MODULE_SENSOR_SENSOR_H_

#include "sensor_types.h"


/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 *
 * \defgroup sensor Sensor
 * @brief Handling of sensor elements
 * @{
 *
 * \defgroup sensor_api API
 * @brief Sensor handling functions
 * @addtogroup sensor_api
 * @{
 */

/**
 * @brief Maximum number of sensors used in the platform
 */
#define MAX_SENSOR 2


/**
 * @brief Initialization of the platform peripherals related to the sensors
 * @param xQueueFromISRToSensorTask Queue to which the ISR should put it's events
 *
 * Configures the sensor related GPIOs and initializes their control structures
 */
void vSensorInitPlatform(xQueueHandle xQueueFromISRToSensorTask);

/**
 * @brief The ISR that handles the change of state of a sensor
 * @param mask
 */
void vSensorHandlerIsr(uint32 mask);

/**
 * @brief Gets the current sensor state
 * @param eSensor the sensor to query
 * @return the sensor state
 */
teSensorState eSensorGetState(teSensorType eSensor);

/**
 * @}
 * @}
 * @}
 */
#endif /* CODE_MODULE_SENSOR_SENSOR_H_ */
