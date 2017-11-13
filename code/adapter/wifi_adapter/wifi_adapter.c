#include "wifi_adapter.h"

teException eWifiConnectToAP(char* pcSsid, char* pcPassword)
{
	teException eException = EX_SUCCESSFUL;
    struct station_config* sConfig = (struct station_config*)zalloc(sizeof(struct station_config));

	espconn_mdns_close();

    sprintf(sConfig->ssid, pcSsid);
    sprintf(sConfig->password, pcPassword);

    if (!wifi_station_set_config(sConfig))
    {
    	SET_EXCEPTION(EX_WIFI_CONFIG)
    	goto end_WifiConnectToAP;
    }

    vWifiSetHostname(STA_NAME);

    if (!wifi_station_connect())
    {
    	SET_EXCEPTION(EX_WIFI_CONNECT)
    	goto end_WifiConnectToAP;
    }
end_WifiConnectToAP:
	free(sConfig);
	return eException;
}

teException eWifiWaitToBeConnected()
{
	teException eException = EX_SUCCESSFUL;
    STATION_STATUS eStatus;
    do
    {
    	eStatus = wifi_station_get_connect_status();
    } while (eStatus != STATION_GOT_IP);
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
	free(config);
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
