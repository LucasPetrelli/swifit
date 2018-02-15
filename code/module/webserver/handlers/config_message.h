/*
 * config_message.h
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_MESSAGE_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_MESSAGE_H_

#include "handler_types.h"

extern const char * zConfigMessageCGI;

char* zHandlerConfigMessage(char* pcRequestData);



#endif /* CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_MESSAGE_H_ */
