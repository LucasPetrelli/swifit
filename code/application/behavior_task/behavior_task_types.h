/*
 * behaviour_task_types.h
 *
 *  Created on: Feb 25, 2018
 *      Author: Lucas
 */

#ifndef CODE_APPLICATION_BEHAVIOR_TASK_BEHAVIOR_TASK_TYPES_H_
#define CODE_APPLICATION_BEHAVIOR_TASK_BEHAVIOR_TASK_TYPES_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "basictypes.h"
#include "mem_types.h"
#include "devices_types.h"
/**
 * @brief Behavior task operation mode
 */

typedef enum {
	NOT_CONNECTED,		//!< Device is not connected to user AP
	NODE,				//!< Device is connected, network already has a head
	HEAD				//!< Device is connected, head of the network
} teBehaviourMode;


/**
 * @brief Behavior task configuration structure
 */
typedef struct {
	uint32_t u32Period;
	portTickType xLastWakeTime;			//!< Last task wake tick time
	xQueueHandle xBehaviorQueue;		//!< Queue from which the task receives commands and info

	teBehaviourMode eMode;				//!< Mode of behavior

	xTimerHandle xBroadcastTimer;
	uint32_t u32BroadcastPeriod;

	uint8_t u8MasterIP_[4];				//!< Contains the master device IP (can be the self if eMode==HEAD)
	xListOfDevices xDeviceList;			//!< List of known devices

	uint32_t u32DeviceTimeoutCounter;	//!< Counter that when a match with u32BroadcastPeriod occurs the timeout check is done
} tsBehaviourTaskConfiguration;






#endif /* CODE_APPLICATION_BEHAVIOR_TASK_BEHAVIOR_TASK_TYPES_H_ */
