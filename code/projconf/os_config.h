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


#define HTTP_TASK_PRIO 5
#define SENSOR_TASK_PRIO 3
#define ACTUATOR_TASK_PRIO 3
#define PLATFORM_TASK_PRIO 2
#define BEHAVIOR_TASK_PRIO 6
#define TIMEKEEPER_TASK_PRIO 3
#define WIFI_TASK_PRIO 4

#endif /* CODE_PROJCONF_OS_CONFIG_H_ */
