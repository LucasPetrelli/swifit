/*
 * @file wifi_adapter.h
 * @author Lucas Petrelli
 * @date Aug 2017
 * @brief Adapter for the ESP8266 SDK's Wifi features
 */

#ifndef WIFI_ADAPTER_H_
#define WIFI_ADAPTER_H_

#include "exceptions.h"
#include "wifi_types.h"
#include "debug_adapter.h"

#include "esp_common.h"
#include "esp_wifi.h"

#include "espconn_mdns.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define AP_SSID "SwifiConfig"
#define AP_PSWD "placeholder"

#define STA_NAME "espsample"

/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 */

/**
 * @addtogroup adapter
 * @{
 */

/**
 * \defgroup wifi_adapter Wifi
 * @brief Wifi setup API
 */

/**
 * @addtogroup wifi_adapter
 * @{
 */

tsWifiControl sWifiControl; /**< Main WIFI control structure */

//-------------------------------------------------------------------------------------------------
/**
  * @brief     Connect to a specified access point with the given password
  *
  * @param     char* pcSsid : null-terminated string for the access point ssid
  * @param 	   char* pcPassword : null-terminated string for the password to be used
  *
  * @return    teException
  */
teException eWifiConnectToAP(char* pcSsid, char* pcPassword);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
  * @brief     blocks the current task until the wifi is connected
  *
  * @param     null
  *
  * @return    teException
  */
teException eWifiWaitToBeConnected(uint8_t u8Timeout);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
  * @brief     Retrieves the current IP, given by the access point
  *
  * @param     uint8_t* pu8Ip : destination for the IP to be written (4 bytes)
  *
  * @return    EX_SUCCESS : operation successful
  * 		   EX_FAIL : operation failed
  */
teException eWifiRetrieveCurrentIP(uint8_t* pu8Ip);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
  * @brief     Retrieves the current wifi connection status
  *
  *
  * @return		WIFI_DISCONNECTED,			Wifi disconnected
  * 			WIFI_CONNECTED_AS_AP,		Wifi connected, working as access point
  *				WIFI_CONNECTED_TO_AP,		Wifi connected, connected to an access point
  */
teWifiConnectionStatus eWifiGetConnectionStatus();
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
  * @brief     Test if wifi is connected or not
  *
  *
  * @return    True if connected, false if not
  */
BOOL bWifiIsWifiConnected();
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief 		Set wifi modem mode
 */
void vWifiSetMode(teWifiMode eMode);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief		Start Access Point operation
 */
void vWifiStartAP();
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * Sets the hostname of the device to be reported to the modem
 * @param zHostname null-terminated string name of the device
 */
void vWifiSetHostname(char* zHostname);
//-------------------------------------------------------------------------------------------------

/**
 * Sets the queue to where the wifi connection state change will put it's events
 * @param xQueue The queue handle
 */
void vWifiSetNotificationQueue(xQueueHandle xQueue);



/**
 * Initialize the mDNS responder
 */
void vWifiInitMDNS();

/**
 * @}
 */

/**
 * @}
 */

#endif /* WIFI_ADAPTER_H_ */
