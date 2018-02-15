/*
 * device_restart.h
 *
 *  Created on: Feb 12, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_DEVICE_RESTART_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_DEVICE_RESTART_H_


#include "handler_types.h"

extern const char * zDeviceRestartCGI;

char* zHandlerDeviceRestart(char* pcRequestData);

#endif /* CODE_MODULE_WEBSERVER_HANDLERS_DEVICE_RESTART_H_ */
