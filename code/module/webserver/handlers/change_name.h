/*
 * change_name.h
 *
 *  Created on: Mar 1, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_CHANGE_NAME_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_CHANGE_NAME_H_

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
 * @brief WLAN configuration post resource string
 */
extern const char * zChangeNameCGI;

/**
 * @brief WLAN configuration post handler
 * @param pcRequestData the HTTP request parsed data
 * @return the HTTP response data, allocated. Must be freed by caller.
 */
char* zHandlerChangeName(char* pcRequestData);

/**
 * @}
 * @}
 * @}
 */




#endif /* CODE_MODULE_WEBSERVER_HANDLERS_CHANGE_NAME_H_ */
