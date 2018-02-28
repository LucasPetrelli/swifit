/**
 * @file webserver_types.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#ifndef CODE_WEBSERVER_TYPES_H_
#define CODE_WEBSERVER_TYPES_H_

//#define URI_CGI_CONFIG_WLAN "connect_wlan"
#define URI_CGI "/cgi/"

#include "basictypes.h"
#include "http_ok.h"
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
 * \defgroup webserver_types Types
 * @brief Webserver type definitions
 * @addtogroup webserver_types
 * @{
 */

/**
 * @brief HTTP method identification
 */
typedef enum {
	HTTP_GET,		//!< HTTP GET method received
	HTTP_POST,		//!< HTTP POST method received
	HTTP_CORRUPT,	//!< Unknown method received
} teRequestType;

/**
 * @brief HTTP request decoded data, separated into relevant fields
 */
typedef struct {
	teRequestType eType;	//!< Type of the request

	char* pcURIData;		//!< Resource requested string
	uint16 u16URILen;		//!< Length of the resource string

	char* pcPostData;		//!< Data posted by the request
	uint16 u16PostLen;		//!< Length of the data posted
} tsHttpRequest;

/**
 * @}
 * @}
 * @}
 */

#endif /* CODE_MODULE_WEBSERVER_WEBSERVER_TYPES_H_ */
