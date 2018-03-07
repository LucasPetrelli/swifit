/*
 * update_rules.h
 *
 *  Created on: Mar 6, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_UPDATE_RULES_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_UPDATE_RULES_H_

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
 * @brief Network rules update resource string
 */
extern const char * zUpdateRulesCGI;

/**
 * @brief Network rules update handler
 * @param pcRequestData the HTTP request parsed data
 * @return the HTTP response data, allocated. Must be freed by caller.
 */
char* zHandlerUpdateRules(char* pcRequestData);

#endif /* CODE_MODULE_WEBSERVER_HANDLERS_UPDATE_RULES_H_ */
