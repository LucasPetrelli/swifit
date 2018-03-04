/**
 * @file http_types.h
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#ifndef CODE_ADAPTER_HTTP_ADAPTER__HTTP_TYPES_H_
#define CODE_ADAPTER_HTTP_ADAPTER__HTTP_TYPES_H_

/**
 * @brief Pointer to HTTP request receive callback function
 *
 * Pointer to HTTP request receive callback function. Should
 * handle the HTTP request. Returns void, has a char* pointing
 * to the request.
 */
typedef void (*vHTTPRequestCallback)(char*);

typedef struct {
	vHTTPRequestCallback vCallback;
	struct netconn *client;
	struct netconn *server;
} tsHTTPControl;
#endif /* CODE_ADAPTER_HTTP_ADAPTER__HTTP_TYPES_H_ */
