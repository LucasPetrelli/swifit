/*
 * config_wlan.h
 *
 *  Created on: Feb 4, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_WLAN_H_
#define CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_WLAN_H_

#include "handler_types.h"

extern const char * zConfigWlanCGI;

char* zHandlerConfigWlan(char* pcRequestData);

#endif /* CODE_MODULE_WEBSERVER_HANDLERS_CONFIG_WLAN_H_ */
