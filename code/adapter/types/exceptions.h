/**
 * @file exceptions.h
 * @author Lucas Petrelli
 * @date Aug 19, 2017
 * @brief Definitions of exception type and macros for handling
 *
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_
#include "debug_adapter.h"

/**
 * @brief Macro for setting an exception while logging the action
 */
#define SET_EXCEPTION( EXC ) { eException=(EXC); LOG_DEBUG("[L %u] EXCEPTION: " #EXC , __LINE__); }


/**
 * @brief Exception types
 */
typedef enum {
	EX_SUCCESSFUL = 0,						/**< Operation successful */
	EX_GENERIC_FAIL = 1,					/**< Unidentified failure */

	EX_WIFI_CONFIG = 0x10,					/**< Failed to config wifi */
	EX_WIFI_CONNECT = 0x11,					/**< Failed to connect to the wifi network assigned */
	EX_WIFI_READ_IP = 0x12,					/**< Failed to read the IP given by the access point */

	EX_UDP_EMPTY_BUFFER = 0x20,				/**< Attempt to pop from empty buffer */

	EX_FILESYSTEM_OPEN_ERROR = 0x30,		/**< Attempt to open a file error */
	EX_FILESYSTEM_RD_ERROR = 0x31,			/**< Attempt to read from a file error */
	EX_FILESYSTEM_WR_ERROR = 0x32			/**< Attempt to write to file error */
} teException;

#endif /* EXCEPTIONS_H_ */
