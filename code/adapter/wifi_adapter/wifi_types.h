/*
 * wifi_types.h
 *
 *  Created on: Oct 8, 2017
 *      Author: Lucas
 */

#ifndef CODE_ADAPTER_WIFI_ADAPTER_WIFI_TYPES_H_
#define CODE_ADAPTER_WIFI_ADAPTER_WIFI_TYPES_H_
#include "basictypes.h"

/**
 *  @brief Wifi connection status enumeration
 */
typedef enum {
	WIFI_DISCONNECTED,			/**< Wifi disconnected */
	WIFI_CONNECTED_AS_AP,		/**< Wifi connected, working as access point */
	WIFI_CONNECTED_TO_AP,		/**< Wifi connected, connected to an access point */
	WIFI_ENUM_SIZE				/**< teWifiConnectionStatus number of elements */
} teWifiConnectionStatus;

/**
 * @brief Wifi modem mode
 */
typedef enum {
	WIFI_STATION,				/**< Wifi modem as station */
	WIFI_AP						/**< Wifi modem as access point */
} teWifiMode;

/**
 * @brief Wifi control structure
 */
typedef struct {
	char* zClientSSID;			/**< User wifi network SSID */
	char* zClientPSWD;			/**< User wifi network password */
	char* zConfigSSID;			/**< SSID of the wifi network when in config mode */

	teWifiMode eMode;					/**< Current wifi operation mode */
	teWifiConnectionStatus eStatus;		/**< Current wifi operation status */
} tsWifiControl;


#endif /* CODE_ADAPTER_WIFI_ADAPTER_WIFI_TYPES_H_ */
