/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/12/1, v1.0 create this file.
*******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "wifi_adapter.h"
#include "udp_adapter.h"
#include "spiffs_adapter.h"
#include "system_adapter.h"

#include "webserver.h"
#include "configuration.h"


#include "wifi_task.h"
#include "sensor_task.h"
#include "actuator_task.h"
#include "platform_task.h"
#include "behavior_task.h"



/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{

    /* need to set opmode before you set config */

    char* pcSSID = "Flink-22A4";
    char* pcPassword = "Falp24gh";
    char* pcName = "Magitek-Bit-001";


    vSystemInit();
    LOG_DEBUG("Init\n\n\n");
    struct	rst_info	*rtc_info	=	system_get_rst_info();
    LOG_DEBUG("reset reason:	%x\n",	rtc_info->reason);
	if	(rtc_info->reason	==	REASON_WDT_RST	||
		rtc_info->reason	==	REASON_EXCEPTION_RST	||
		rtc_info->reason	==	REASON_SOFT_WDT_RST)	{
		if	(rtc_info->reason	==	REASON_EXCEPTION_RST)	{

			LOG_DEBUG("Fatal exception (%d):\n", rtc_info->exccause);

		}

		LOG_DEBUG(
				"epc1=0x%08x,	"
				"epc2=0x%08x,	"
				"epc3=0x%08x,	"
				"excvaddr=0x%08x,	"
				"depc=0x%08x\n",
				rtc_info->epc1,
				rtc_info->epc2,
				rtc_info->epc3,
				rtc_info->excvaddr,
				rtc_info->depc);//The	address	of	the	last	crash	is	printed,	which	is	used	to	debug	garbled	output.
	}

	// --- Read configuration from the fs
    vConfigurationRead();

	// --- Webserver setup (+Task)
    vSetupWebserver();

    // --- Wifi Task
    xTaskCreate(&vTaskWifi, "Wifi Task", 512, NULL, 4, NULL);

    // --- Sensor Task
    xTaskCreate(&vSensorTask, "Sensor", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // --- Actuator Task
    xTaskCreate(&vActuatorTask, "Actuator", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // --- Platform Task
    tsPlatformTaskConfiguration* psPlatformTask = (tsPlatformTaskConfiguration*) zalloc (sizeof(tsPlatformTaskConfiguration));
    psPlatformTask->u32Period = 100;
    psPlatformTask->u32HeapReportPeriod = 5000;
    xTaskCreate(&vPlatformTask, "Platform", configMINIMAL_STACK_SIZE, (void*) psPlatformTask, 2, NULL);

    // --- Behavior Task
    tsBehaviourTaskConfiguration* psBehaviourTask = (tsBehaviourTaskConfiguration*) zalloc (sizeof(tsBehaviourTaskConfiguration));
    psBehaviourTask->u32Period = 100;
    psBehaviourTask->u32BroadcastPeriod = 1000;
    psBehaviourTask->xBehaviorQueue = xQueueCreate(10, sizeof(tsMemQueueMessage));
    psBehaviourTask->xBroadcastTimer = xTimerCreate(
    		"Broadcast",
			psBehaviourTask->u32BroadcastPeriod/portTICK_RATE_MS,
			pdTRUE,
			(void*) 1,
			vBehaviourBroadcastCallback
	);
    xTaskCreate(&vBehaviorTask, "Behavior", 512, (void*) psBehaviourTask, 5, NULL);

    // --- UDP ISR
	eUDPInit(psBehaviourTask->xBehaviorQueue);

}

