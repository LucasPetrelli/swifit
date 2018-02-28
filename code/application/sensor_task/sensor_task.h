/*
 * sensor_task.h
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_SENSOR_TASK_SENSOR_TASK_H_
#define CODE_APPLICATION_SENSOR_TASK_SENSOR_TASK_H_

#include "sensor.h"

/**
 * \defgroup application Application
 * @brief Application tasks that handle the proper feature implementation
 * @addtogroup application
 * @{
 *
 * \defgroup sensortask Sensor
 * @brief Task that receives sensor events and handles them accordingly
 * @addtogroup sensortask
 * @{
 */

void vSensorTask(void* pvParameters);

/**
 * @}
 * @}
 */

#endif /* CODE_APPLICATION_SENSOR_TASK_SENSOR_TASK_H_ */
