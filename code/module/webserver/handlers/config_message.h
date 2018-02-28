/*
 * config_message.h
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_MESSAGE_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_MESSAGE_H_

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
 * @brief Configuration message get resource string
 */
extern const char * zConfigMessageCGI;

/**
 * @brief Configuration message get handler
 * @param pcRequestData the HTTP request parsed data
 * @return the HTTP response data, allocated. Must be freed by caller.
 *
 * Handles a query from the client asking for the last configuration message available.
 * The message will be deleted from the flash upon retrieval.
 */
char* zHandlerConfigMessage(char* pcRequestData);



#endif /* CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_MESSAGE_H_ */
