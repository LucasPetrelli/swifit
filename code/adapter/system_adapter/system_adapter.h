/**
 * @file system_adapter.h
 * @author Lucas
 * @date Nov 9, 2017
 *
 * @brief ESP8266 System adapter implementation header file
 */

#ifndef CODE_ADAPTER_SYSTEM_ADAPTER_SYSTEM_ADAPTER_H_
#define CODE_ADAPTER_SYSTEM_ADAPTER_SYSTEM_ADAPTER_H_

// Adapter includes
#include "debug_adapter.h"

// Platform includes
#include "esp_common.h"
#include "espconn.h"
#include "uart.h"


/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 *
 * @addtogroup adapter
 * @{
 *
 * \defgroup system_adapter System
 * @brief System miscellaneous APIs
 *
 * @addtogroup system_adapter
 * @{
 *
 */

/**
 * @brief System specific setup encapsulation
 */
void vSystemInit();

/**
 * @brief Get the current amount free RAM bytes
 * @return The number of bytes available on the heap (used in malloc for example)
 */
uint32_t u32SystemFreeHeap();

/**
 * @brief Call to cause a system reset
 */
void vSystemReset();

/**
 * @brief Gets the system uptime in microsseconds
 * @return System uptime in microsseconds
 */
uint32_t u32SystemUptime();

/**
 * @brief Gets an identification number
 * @return The chip identification number, based on the MAC address
 */
uint32_t u32SystemGetId();

/**
 * @brief Gets an random number from the system
 * @return a pseudo random number
 */
uint32_t u32SystemRandom();


/**
 * @}
 * @}
 */

#endif /* CODE_ADAPTER_SYSTEM_ADAPTER_SYSTEM_ADAPTER_H_ */
