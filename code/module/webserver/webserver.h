/**
 * @file webserver.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#ifndef CODE_MODULE_WEBSERVER_WEBSERVER_H_
#define CODE_MODULE_WEBSERVER_WEBSERVER_H_

#include "http_adapter.h"
#include "webserver_types.h"
#include "debug_adapter.h"

/**
 * @Brief Setup Webserver functionality
 *
 * Setups webserver functionality, starting the HTTP handler task and
 * assigning a callback to the HTTP control structure to
 */
void vSetupWebserver();

/**
 * @brief HTTP request handler
 *
 * Handles a HTTP, parsing it's data and replying accordingly
 */
void vRequestHandler(char* zRequest);

/**
 * @brief Parser to get data from request
 */
tsHttpRequest sGetRequest(char* zRequest);



#endif /* CODE_MODULE_WEBSERVER_WEBSERVER_H_ */
