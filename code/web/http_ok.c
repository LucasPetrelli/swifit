/*
 * http_ok.c
 *
 *  Created on: Feb 4, 2018
 *      Author: Lucas
 */

#include "c_types.h"
#include "http_ok.h"

const char* zHttpHeaderOK = {
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n\r\n"
		""
};

const char ICACHE_RODATA_ATTR STORE_ATTR zHttpNotFound[] = {
		"HTTP/1.0 404 \r\n\r\n"
};
