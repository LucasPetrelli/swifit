#include "wifi_adapter.h"
#include "mem_types.h"

xQueueHandle xWifiNotifyQueue;

void prv_vWifiStateChangeHandler(System_Event_t* event);

teException eWifiConnectToAP(char* pcSsid, char* pcPassword)
{
	teException eException = EX_SUCCESSFUL;
    struct station_config* sConfig = (struct station_config*)zalloc(sizeof(struct station_config));

	//espconn_mdns_close();

    sprintf(sConfig->ssid, pcSsid);
    sprintf(sConfig->password, pcPassword);

    if (!wifi_station_set_config(sConfig))
    {
    	SET_EXCEPTION(EX_WIFI_CONFIG)
    	goto end_WifiConnectToAP;
    }

    if (!wifi_station_connect())
    {
    	SET_EXCEPTION(EX_WIFI_CONNECT)
    	goto end_WifiConnectToAP;
    }
end_WifiConnectToAP:
	free(sConfig);
	return eException;
}

teException eWifiWaitToBeConnected(uint8_t u8Timeout)
{
	teException eException = EX_SUCCESSFUL;
    STATION_STATUS eStatus;
    do
    {
    	vTaskDelay(1000/portTICK_RATE_MS);
    	eStatus = wifi_station_get_connect_status();

    	u8Timeout -= 1;
    	if (u8Timeout == 0){
    		break;
    	}

    } while (eStatus != STATION_GOT_IP);

    if (eStatus != STATION_GOT_IP)
    {
    	LOG_DEBUG("Connection timeout [%u]", eStatus);
    	switch (eStatus)
    	{
    	case STATION_WRONG_PASSWORD:
    	{
    		SET_EXCEPTION(EX_WIFI_WRONG_PASSWORD);
    		break;
    	}
    	case STATION_NO_AP_FOUND:
    	{
    		SET_EXCEPTION(EX_WIFI_AP_NOT_FOUND);
    		break;
    	}
    	default:
    	{
    		SET_EXCEPTION(EX_GENERIC_FAIL);
    		break;
    	}
    	}
    }

end_WifiWaitToBeConnected:
	return eException;
}

teException eWifiRetrieveCurrentIP(uint8_t* pu8Ip)
{
	teException eException = EX_SUCCESSFUL;
    struct ip_info* sIp = (struct ip_info*)zalloc(sizeof(struct ip_info));
    if (!wifi_get_ip_info(STATION_IF, sIp))
    {
    	SET_EXCEPTION(EX_WIFI_READ_IP)
    	goto end_WifiRetrieveCurrentIP;
    }
    memcpy(pu8Ip, (uint8*)&(sIp->ip.addr), 4);
end_WifiRetrieveCurrentIP:
	free(sIp);
	return eException;
}

teWifiConnectionStatus eWifiGetConnectionStatus()
{
	teWifiConnectionStatus eWifiStatus;
	//TODO: check status as AP
	STATION_STATUS eStatus = wifi_station_get_connect_status();
	if (eStatus == STATION_GOT_IP)
	{
		eWifiStatus = WIFI_CONNECTED_TO_AP;
	}
	else
	{
		eWifiStatus = WIFI_DISCONNECTED;
	}

	return eWifiStatus;
}

BOOL bWifiIsWifiConnected()
{
	espconn_mdns_disable();
	teWifiConnectionStatus eStatus = eWifiGetConnectionStatus();
	if (eStatus != WIFI_DISCONNECTED)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void vWifiSetMode(teWifiMode eMode)
{
	WIFI_MODE eEspMode = STATION_MODE;
	wifi_station_set_auto_connect(false);
	wifi_station_set_reconnect_policy(true);
	if (eMode == WIFI_AP)
	{
		eEspMode = SOFTAP_MODE;
	}
    wifi_set_opmode(eEspMode);
}

void vWifiStartAP()
{
	struct softap_config *config = (struct softap_config *)zalloc(sizeof(struct softap_config)); // initialization
	wifi_softap_get_config(config);

	sprintf(config->ssid, AP_SSID);
	sprintf(config->password, AP_PSWD);

	config->authmode = AUTH_WPA_WPA2_PSK;
	config->ssid_len = 0; // or its actual SSID length
	config->max_connection = 3;

	wifi_softap_set_config_current(config);
	//free(config);
}

void vWifiSetHostname(char* zHostname)
{
	if (!wifi_station_set_hostname(zHostname))
	{
		LOG_DEBUG("Error while setting hostname to %s", zHostname);
	}
}
struct mdns_info info;
void vWifiInitMDNS()
{
	struct ip_info ipconfig;
	wifi_get_ip_info(STATION_IF, &ipconfig);

	mDNSresp_init(0, 0xFFFF);
	mDNSresp_addhost("swifit.local",&ipconfig.ip);
}

void vWifiSetNotificationQueue(xQueueHandle xQueue)
{
	xWifiNotifyQueue = xQueue;
	wifi_set_event_handler_cb(prv_vWifiStateChangeHandler);
}

void prv_vWifiStateChangeHandler(System_Event_t* event)
{
	LOG_DEBUG("Wifi event: [%u]", event->event_id);
	tsMemQueueMessage sMsg;
	switch (event->event_id)
	{
	case EVENT_STAMODE_SCAN_DONE:
	{
		LOG_DEBUG("EVENT_STAMODE_SCAN_DONE");
		break;
	}
	case EVENT_STAMODE_CONNECTED:
	{
		LOG_DEBUG("EVENT_STAMODE_CONNECTED");
		break;
	}
	case EVENT_STAMODE_DISCONNECTED:
	{
		LOG_DEBUG("EVENT_STAMODE_DISCONNECTED [reason %u]", event->event_info.disconnected.reason);
		teWifiConnectionStatus eStatus = WIFI_DISCONNECTED;
		sMsg.eType = WIFI_MESSAGE;
		sMsg.pvData = (void*) eStatus;
		xQueueSendFromISR(xWifiNotifyQueue ,(void*) &sMsg, NULL);
		break;
	}
	case EVENT_STAMODE_AUTHMODE_CHANGE:
	{
		LOG_DEBUG("EVENT_STAMODE_AUTHMODE_CHANGE");
		break;
	}
	case EVENT_STAMODE_GOT_IP:
	{
		LOG_DEBUG("EVENT_STAMODE_GOT_IP");
		teWifiConnectionStatus eStatus = WIFI_CONNECTED_TO_AP;
		sMsg.eType = WIFI_MESSAGE;
		sMsg.pvData = (void*) eStatus;
		xQueueSendFromISR(xWifiNotifyQueue ,(void*) &sMsg, NULL);
		break;
	}
	case EVENT_STAMODE_DHCP_TIMEOUT:
	{
		LOG_DEBUG("EVENT_STAMODE_DHCP_TIMEOUT");
		break;
	}
	case EVENT_SOFTAPMODE_STACONNECTED:
	{
		LOG_DEBUG("EVENT_SOFTAPMODE_STACONNECTED");
		break;
	}
	case EVENT_SOFTAPMODE_STADISCONNECTED:
	{
		LOG_DEBUG("EVENT_SOFTAPMODE_STADISCONNECTED");
		break;
	}
	case EVENT_SOFTAPMODE_PROBEREQRECVED:
	{
		LOG_DEBUG("EVENT_SOFTAPMODE_PROBEREQRECVED");
		break;
	}
	}
}
