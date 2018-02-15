/*
 * status.h
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_STATUS_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_STATUS_H_

#include "handler_types.h"

extern const char * zStatusCGI;

char* zHandlerStatus(char* pcRequestData);

#endif /* CODE_MODULE_WEBSERVER_HANDLERS_STATUS_H_ */
