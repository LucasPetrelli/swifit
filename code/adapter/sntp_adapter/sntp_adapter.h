/*
 * sntp_adapter.h
 *
 *  Created on: Mar 2, 2018
 *      Author: Lucas
 */

#ifndef CODE_ADAPTER_SNTP_ADAPTER_SNTP_ADAPTER_H_
#define CODE_ADAPTER_SNTP_ADAPTER_SNTP_ADAPTER_H_

/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 *
 * @addtogroup adapter
 * @{
 *
 * \defgroup sntp_adapter SNTP
 * @brief Simple Network Time Protocol adaptation layer
 *
 * Adapts the ESP8266 RTOS SDK SNTP implementation (borrowed from lwIP)
 *
 * @addtogroup sntp_adapter
 * @{
 */
#include "basictypes.h"
/**
 * @brief Does the required initialization for the SNTP functionality
 */
void vSNTPInit();

/**
 * @brief Retrieves the current timestamp from a registered SNTP server
 * @return The GMT-3 timestamp
 */
uint32_t u32SNTPGetTimestamp();

/**
 * @}
 * @}
 */



#endif /* CODE_ADAPTER_SNTP_ADAPTER_SNTP_ADAPTER_H_ */
