/*
 * os_config.h
 *
 *  Created on: Mar 2, 2018
 *      Author: Lucas
 */

#ifndef CODE_PROJCONF_OS_CONFIG_H_
#define CODE_PROJCONF_OS_CONFIG_H_

typedef enum {
	HTTP_TASK,
	SENSOR_TASK,
	ACTUATOR_TASK,
	PLATFORM_TASK,
	BEHAVIOR_TASK,
	TIMEKEEPER_TASK,
	WIFI_TASK,
	N_TASKS
} teTaskId;

#endif /* CODE_PROJCONF_OS_CONFIG_H_ */
