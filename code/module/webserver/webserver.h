/**
 * @file webserver.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#ifndef CODE_MODULE_WEBSERVER_WEBSERVER_H_
#define CODE_MODULE_WEBSERVER_WEBSERVER_H_

#define WEBSV_VERBOSE
#ifdef WEBSV_VERBOSE
#define LOG_WEB(args...) LOG_DEBUG(args)
#else
#define LOG_WEB(args...) { do {} while(0); }
#endif

#include "webserver_types.h"
#include "welcome.html.h"
#include "http_adapter.h"
#include "debug_adapter.h"
#include "cJSON.h"

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


/**
 * @brief reads the decoded request data,
 * @param sRequest decoded request structure
 * @return the pointer to the response (in chars)
 */
char* pcHandleDecodedRequest(tsHttpRequest sRequest);


#endif /* CODE_MODULE_WEBSERVER_WEBSERVER_H_ */
