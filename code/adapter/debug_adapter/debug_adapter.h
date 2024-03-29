/**
 * @file debug_adapter.h
 * @author Lucas
 * @date Oct 9, 2017
 *
 * @brief Debug and logging tools implementation
 */

#ifndef CODE_ADAPTER_DEBUG_ADAPTER_DEBUG_ADAPTER_H_
#define CODE_ADAPTER_DEBUG_ADAPTER_DEBUG_ADAPTER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "../../projconf/product_config.h"


/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 *
 * @addtogroup adapter
 * @{
 *
 * \defgroup debug_adapter Debug
 * @brief Debugging handling APIs
 * @addtogroup debug_adapter
 * @{
 */


/**
 * @brief print macro mutex
 */
extern xSemaphoreHandle xPrintSemaphore;


/**
 * @brief Serial print macro
 *
 * Will be replaced by nothing when the DEBUG symbol is not defined
 */
#ifdef DEBUG
	#define LOG_DEBUG(args...) {xSemaphoreTake(xPrintSemaphore, portMAX_DELAY);  printf("[%s:%u]", __FILE__, __LINE__);printf(args);printf("\n"); xSemaphoreGive(xPrintSemaphore);}
#else
	#define LOG_DEBUG(args...) {do {} while(0);}
#endif

/**
 * @brief Debug initialization
 *
 * Initializes variables related to debugging
 */
void vDebugInit(void);

/**
 * @}
 * @}
 */

#endif /* CODE_ADAPTER_DEBUG_ADAPTER_DEBUG_ADAPTER_H_ */
