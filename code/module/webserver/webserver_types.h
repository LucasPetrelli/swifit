/**
 * @file webserver_types.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#ifndef CODE_MODULE_WEBSERVER_WEBSERVER_TYPES_H_
#define CODE_MODULE_WEBSERVER_WEBSERVER_TYPES_H_

typedef enum {
	HTTP_GET,
	HTTP_POST,
	HTTP_CORRUPT,
} teRequestType;

typedef struct {
	teRequestType eType;

	char* pcURIData;
	uint16 u16URILen;

	char* pcPostData;
	uint16 u16PostLen;
} tsHttpRequest;

#endif /* CODE_MODULE_WEBSERVER_WEBSERVER_TYPES_H_ */
