/**
 * @file http_adapter.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#ifndef CODE_TESTING_TESTHTTP_HTTP_ADAPTER_H_
#define CODE_TESTING_TESTHTTP_HTTP_ADAPTER_H_
#include <string.h>
#include <lwip/api.h>

#include "freertos/FreeRTOS.h"
#include "debug_adapter.h"
#include "http_types.h"

//#define HTTP_DEBUG

#ifdef HTTP_DEBUG
#define LOG_HTTP(args...) LOG_DEBUG(args);
#else
#define LOG_HTTP(args...) {do {} while(0);}
#endif

/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 */

/**
 * @addtogroup adapter
 * @{
 */

/**
 * \defgroup http_adapter HTTP
 * @brief HTTP server platform specific implementation
 *
 * Has functions to start a http server task and request handler
 */

/**
 * @addtogroup http_adapter
 * @{
 */


/**
 * @brief This module uses the ESP8266 lwIP implementation
 * 			to make a HTTP server
 */

/**
 * @brief HTTP server task
 *
 * Keeps listening on TCP port 80 for HTTP requests, calling the function set by
 * vHTTPSetCallback to handle.
 */
void vHTTPTask(void* pvParameters);

/**
 * @brief Sets the HTTP request handler function
 * 		  Should make use of vHTTPSendAnswer to answer the request
 */
void vHTTPSetCallback(vHTTPRequestCallback vFunc);

/**
 * @brief Send an answer to the last recent HTTP request
 *
 * @param zAnswer: null-terminated string representing the answer
 */
void vHTTPSendAnswer(char* zAnswer);

/**
 * @}
 */

/**
 * @}
 */

#endif /* CODE_TESTING_TESTHTTP_HTTP_ADAPTER_H_ */
