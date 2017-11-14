/*
 * sensor.h
 *
 *  Created on: Nov 13, 2017
 *      Author: Lucas
 */

#ifndef CODE_MODULE_SENSOR_SENSOR_H_
#define CODE_MODULE_SENSOR_SENSOR_H_

#include "sensor_types.h"


#define MAX_SENSOR 2
void vSensorInitPlatform(xQueueHandle xQueueFromISRToSensorTask);
void vSensorHandlerIsr(uint32 mask);

#endif /* CODE_MODULE_SENSOR_SENSOR_H_ */
