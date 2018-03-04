/*
 * update_timing.h
 *
 *  Created on: Mar 3, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_UPDATE_TIMING_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_UPDATE_TIMING_H_
#include "handler_types.h"

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 *
 * \defgroup webserver Webserver
 * @brief Handling of the web user interface
 * @{
 *
 * \defgroup webserver_handlers Handlers
 * @brief Webserver request handler functions
 * @addtogroup webserver_handlers
 * @{
 */

/**
 * @brief Device timing request resource string
 */
extern const char * zUpdateTimingCGI;

/**
 * @brief Device timing update handler
 * @param pcRequestData the HTTP request parsed data
 * @return the HTTP response data
 */
char* zHandlerUpdateTiming(char* pcRequestData);

/**
 * @}
 * @}
 * @}
 */



#endif /* CODE_MODULE_WEBSERVER_HANDLERS_UPDATE_TIMING_H_ */
