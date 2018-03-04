/**
 * @file webserver.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#ifndef CODE_MODULE_WEBSERVER_WEBSERVER_H_
#define CODE_MODULE_WEBSERVER_WEBSERVER_H_

//Type definition
#include "webserver_types.h"
#include "handler_types.h"

//Webpages
#include "welcome_config.html.h"
#include "homepage.html.h"
#include "homepage.css.h"
#include "homepage.js.h"
#include "devices_page.js.h"
#include "devices_page_template.js.h"
#include "timing_page.js.h"
#include "timing_page_template.js.h"
#include "timing_page.css.h"

//Used adapters
#include "http_adapter.h"
#include "debug_adapter.h"

//Other modules
#include "devices_types.h"

//Third party
#include "cJSON.h"

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
 * \defgroup webserver_api API
 * @brief Webserver handling functions
 * @addtogroup webserver_api
 * @{
 */


//#define WEBSV_VERBOSE

/**
 * @brief Local logger function definition
 *
 * This macro prints only when the WEBSV_VERBOSE symbol is defined
 */
#ifdef WEBSV_VERBOSE
#define LOG_WEB(args...) LOG_DEBUG(args)
#else
#define LOG_WEB(args...) { do {} while(0); }
#endif


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
 * Handles a HTTP request, parsing it's data and replying accordingly
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

/**
 * @brief generates a JSON representation of the current state of the device
 * @param psDev a pointer to the device to be transformed into JSON
 * @return a pointer to the JSON object
 */
cJSON* psWebserverMakeJSONFromDevice(tsDevice* psDev);


/**
 * @brief generates a JSON representation of the timing table of the device
 * @param psDev a pointer to the device whose time table is to be transformed into JSON
 * @return a pointer to the JSON object
 */
cJSON* psWebserverMakeJSONFromTiming(tsDevice* psDev);


/**
 * @brief deletes a JSON object
 * @param pointer to the JSON object
 */
void vWebserverDeleteJSON(cJSON* psJSON);


/**
 * @brief Homepage retrieval function
 * @return a pointer to the homepage html/css/js code, as a null terminated string
 *
 * Retrieves the home page suitable for the current mode (configuration or normal operation). It allocated the
 * needed memory and returns a pointer to the allocated data. The caller must handle the deallocation of the memory.
 */
char* zGetHomepage();

#endif /* CODE_MODULE_WEBSERVER_WEBSERVER_H_ */
