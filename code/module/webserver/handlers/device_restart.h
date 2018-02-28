/*
 * device_restart.h
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_DEVICE_RESTART_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_DEVICE_RESTART_H_


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
 * @brief Device restart post resource string
 */
extern const char * zDeviceRestartCGI;

/**
 * @brief Device restart post handler
 * @param pcRequestData the HTTP request parsed data
 * @return the HTTP response data, allocated. Must be freed by caller.
 *
 * Schedules a system reset in 1 second.
 */
char* zHandlerDeviceRestart(char* pcRequestData);

/**
 * @}
 * @}
 * @}
 */
#endif /* CODE_MODULE_WEBSERVER_HANDLERS_DEVICE_RESTART_H_ */
