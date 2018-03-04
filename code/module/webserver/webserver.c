/**
 * @file webserver.c
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */
#include <stdio.h>
#include <stdlib.h>

#include "webserver.h"
#include "configuration.h"
#include "actuator.h"
#include "sensor.h"
#include "devices.h"
#include "timing.h"
#include "cJSON.h"

cJSON_Hooks sJSONHooks ;

char* prv_pcHandleCGIRequest(const tsHttpRequest* sRequest, char* pcResponse);
char* prv_zReadHomePageFromFlash(char* address);

xTaskHandle vSetupWebserver()
{
	// Set the callback that handles an received HTTP request
	vHTTPSetCallback((vHTTPRequestCallback) vRequestHandler);

	// Launch the HTTP listener task
	// It calls the callback configured above when an HTTP request is received
	xTaskHandle xCreatedTask;
    xTaskCreate(&vHTTPTask, "http_server", 256+64, NULL, 4, &xCreatedTask);
    return xCreatedTask;
}

void vRequestHandler(char* zRequest)
{
	//LOG_WEB("New request!\n%s", zRequest);

	// Parses the raw request into a structure with data split
    tsHttpRequest sParsedRequest = sGetRequest(zRequest);

    // Handles the request
    char* pcResponse = pcHandleDecodedRequest(sParsedRequest);
    if (pcResponse)
    {
		vHTTPSendAnswer(pcResponse);
		free(pcResponse);
    }
}

char* pcHandleDecodedRequest(tsHttpRequest sRequest)
{
	char* pcResponse = NULL;

	// If no URI was requested, answer with the home page
	// The function looks into the current mode and selects the appropriate page
	if (sRequest.u16URILen == 1)
	{
		pcResponse = zGetHomepage();
	}
	else if (strncmp(sRequest.pcURIData, URI_CGI, strlen(URI_CGI))==0)
	{
		pcResponse = prv_pcHandleCGIRequest(&sRequest, pcResponse);
	}
	else
	{
		if (strncmp(sRequest.pcURIData, acHomepageJsId, strlen(acHomepageJsId)) == 0)
		{
			pcResponse = prv_zReadHomePageFromFlash((char*) acHomepageJs);
		}
		else if (strncmp(sRequest.pcURIData, acHomepageCssId, strlen(acHomepageCssId)) == 0)
		{
			pcResponse = prv_zReadHomePageFromFlash((char*) acHomepageCss);
		}
		else if (strncmp(sRequest.pcURIData, acDevices_pageJsId, strlen(acDevices_pageJsId)) == 0)
		{
			pcResponse = prv_zReadHomePageFromFlash((char*) acDevices_pageJs);
		}
		else if (strncmp(sRequest.pcURIData, acTiming_pageJsId, strlen(acTiming_pageJsId)) == 0)
		{
			pcResponse = prv_zReadHomePageFromFlash((char*) acTiming_pageJs);
		}
		else if (strncmp(sRequest.pcURIData, acTiming_pageCssId, strlen(acTiming_pageCssId)) == 0)
		{
			pcResponse = prv_zReadHomePageFromFlash((char*) acTiming_pageCss);
		}
		else if (strncmp(sRequest.pcURIData, acDevices_page_templateJsId, strlen(acDevices_page_templateJsId)) == 0)
		{
			pcResponse = prv_zReadHomePageFromFlash((char*) acDevices_page_templateJs);
		}
		else if (strncmp(sRequest.pcURIData, acTiming_page_templateJsId, strlen(acTiming_page_templateJsId)) == 0)
		{
			pcResponse = prv_zReadHomePageFromFlash((char*) acTiming_page_templateJs);
		}
	}

pcHandleDecodedRequest_exit:
	return pcResponse;
}

char* prv_pcHandleCGIRequest(const tsHttpRequest* sRequest, char* pcResponse)
{
	LOG_WEB("CGI requested");
	char* p = sRequest->pcURIData + strlen(URI_CGI);
	char* zCGI = (char*) zalloc(sRequest->u16URILen - strlen(URI_CGI) + 1);
	char* data = (char*) zalloc(sRequest->u16PostLen + 1);
	strncpy(zCGI, p, sRequest->u16URILen - strlen(URI_CGI));
	strncpy(data, sRequest->pcPostData, sRequest->u16PostLen);
	LOG_DEBUG("%s requested\nData: %s", zCGI, data);

	if (strncmp(p, zConfigWlanCGI, strlen(zConfigWlanCGI)) == 0)
	{
		pcResponse = zHandlerConfigWlan(data);
	}
	else if (strncmp(p, zStatusCGI, strlen(zStatusCGI)) == 0)
	{
		pcResponse = zHandlerStatus(data);
	}
	else if (strncmp(p, zConfigMessageCGI, strlen(zConfigMessageCGI)) == 0)
	{
		pcResponse = zHandlerConfigMessage(data);
	}
	else if (strncmp(p, zDeviceRestartCGI, strlen(zDeviceRestartCGI)) == 0)
	{
		pcResponse = zHandlerDeviceRestart(data);
	}
	else if (strncmp(p, zDeviceListCGI, strlen(zDeviceListCGI)) == 0)
	{
		pcResponse = zHandlerDeviceList(data);
	}
	else if (strncmp(p, zIssueActionCGI, strlen(zIssueActionCGI)) == 0)
	{
		pcResponse = zHandlerIssueAction(data);
	}
	else if (strncmp(p, zDevicesTimingCGI, strlen(zDevicesTimingCGI)) == 0)
	{
		pcResponse = zHandlerDevicesTiming(data);
	}
	else if (strncmp(p, zUpdateTimingCGI, strlen(zUpdateTimingCGI)) == 0)
	{
		pcResponse = zHandlerUpdateTiming(data);
	}

	free(zCGI);
	free(data);
	return pcResponse;
}

char* prv_zReadHomePageFromFlash(char* address)
{
	uint32_t* page = NULL;
	uint32_t u32ByteRead;
	uint32_t u32StrLen = 0;
	char* startingAddress = address;
	uint32_t u32AmountCopied = 0;
	uint32_t* u32WrPtr = NULL;

	//find strlen

	u32ByteRead = *((uint32_t*)address);
	//LOG_DEBUG("Strlen mask 0x%08x", ((uint32_t)~0x03));
	//LOG_DEBUG("RD 0x%08x", u32ByteRead);
	while (u32ByteRead & ((uint32_t)0x000000FF))
	{
		u32ByteRead >>= 8;
		u32StrLen += 1;

		if ((u32StrLen&((uint32_t)0x03))==0)
		{
			address += 4;
			u32ByteRead = *((uint32_t*)address);
		}

	}
	//ceil strlen to a multiple of 4
	u32StrLen += 1;
	u32StrLen += 4 - u32StrLen%4;
	//LOG_DEBUG("Webpage size = %u", u32StrLen);
	//allocate u32StrLen+1
	page = (uint32_t*) zalloc(u32StrLen);
	u32WrPtr = page;
	for (u32AmountCopied=0; u32AmountCopied<u32StrLen; u32AmountCopied+=4)
	{
		//LOG_DEBUG("ADDR 0x%08x = 0x%08x", u32WrPtr, *((uint32_t*) (startingAddress+u32AmountCopied)));
		*u32WrPtr = *((uint32_t*) (startingAddress+u32AmountCopied));
		u32WrPtr +=1;
	}
	//LOG_DEBUG("%u copied", strlen( (char*)page));
	//LOG_DEBUG("%s", (char*)page);
	return (char*)page;
}

char* zGetHomepage()
{
	char* pcHomepage = NULL;
	tsConfiguration* psDeviceConfig = psConfigurationGet();
	if (psDeviceConfig->eMode == CONFIG_MODE)
	{
		pcHomepage = prv_zReadHomePageFromFlash((char*) acWelcome_configHtml);
	}
	else
	{
		pcHomepage = prv_zReadHomePageFromFlash((char*) acHomepageHtml);
	}
	return pcHomepage;
}

cJSON* psWebserverMakeJSONFromDevice(tsDevice* psDev)
{
	// create object to represent self
	cJSON* psJSONDevice = cJSON_CreateObject();

	// create itens for represent self information
	char cId_[11];
	sprintf(cId_, "%u", psDev->u32ID);

	cJSON_AddStringToObject(psJSONDevice, "name", psDev->cName_);
	cJSON_AddStringToObject(psJSONDevice, "id", cId_);
	if (psDev->eType == DEVICE_SWITCH)
	{
		cJSON_AddStringToObject(psJSONDevice, "type", "Switch");
		cJSON_AddFalseToObject(psJSONDevice, "hasPir");
	}
	else if (psDev->eType == DEVICE_SOCKET)
	{
		cJSON_AddStringToObject(psJSONDevice, "type", "Plug");
		cJSON_AddTrueToObject(psJSONDevice, "hasPir");

		teSensorState ePIRState = psDev->eSensorState;
		if (ePIRState == TRIGGERED)
		{
			cJSON_AddTrueToObject(psJSONDevice, "pirStatus");
		}
		else
		{
			cJSON_AddFalseToObject(psJSONDevice, "pirStatus");
		}

		teActuatorState eRelayState = psDev->eActuatorState;
		if (eRelayState == ACTIVATED)
		{
			cJSON_AddTrueToObject(psJSONDevice, "relayStatus");
		}
		else
		{
			cJSON_AddFalseToObject(psJSONDevice, "relayStatus");
		}

	}
	return psJSONDevice;
}

cJSON* psWebserverMakeJSONFromTiming(tsDevice* psDev)
{
	// create object to represent psDev
	cJSON* psJSONDevice = cJSON_CreateObject();

	// create itens for represent self information
	char cId_[11];
	sprintf(cId_, "%u", psDev->u32ID);
	cJSON_AddStringToObject(psJSONDevice, "name", psDev->cName_);
	cJSON_AddStringToObject(psJSONDevice, "id", cId_);
	cJSON* psJSONTimeTable = cJSON_CreateArray();

	uint8_t u8TableEntryN = 0;
	for (;u8TableEntryN < N_TIME_ENTRIES; u8TableEntryN++)
	{
		cJSON* psNewEntry = cJSON_CreateObject();
		tsTimingEntry* psEntry = &(psDev->sTimeTable_[u8TableEntryN]);
		char cMinutes_[5];
		sprintf(cMinutes_, "%u", psEntry->u16Minutes);

		cJSON_AddStringToObject(psNewEntry, "time", cMinutes_);
		if (psEntry->eEnabled)
		{
			cJSON_AddTrueToObject(psNewEntry, "enabled");
		}
		else
		{
			cJSON_AddFalseToObject(psNewEntry, "enabled");
		}

		if (psEntry->eAction)
		{
			cJSON_AddTrueToObject(psNewEntry, "action");
		}
		else
		{
			cJSON_AddFalseToObject(psNewEntry, "action");
		}
		cJSON_AddItemToArray(psJSONTimeTable, psNewEntry);
	}
	cJSON_AddItemToObject(psJSONDevice, "times", psJSONTimeTable);
	return psJSONDevice;


}

void vWebserverDeleteJSON(cJSON* psJSON)
{
	cJSON_Delete(psJSON);
}

tsHttpRequest sGetRequest(char* zRequest)
{
	tsHttpRequest sParsedRequest;
	char* pcAux1;
	char* pcAux2;

	// Get request type
	if (!strncmp(zRequest, "GET", 3))
	{
		sParsedRequest.eType = HTTP_GET;
	}
	else if (!strncmp(zRequest, "POST", 4))
	{
		sParsedRequest.eType = HTTP_POST;
	}
	else
	{
		sParsedRequest.eType = HTTP_CORRUPT;
		LOG_DEBUG("Request Corrupted");
		return sParsedRequest;
	}

	// Get request URI
	pcAux1=strchr(zRequest, ' ');
	pcAux1 =(char*)((uint32)pcAux1 + 1);


	pcAux2=strchr(pcAux1, ' ');
	sParsedRequest.pcURIData = pcAux1;
	sParsedRequest.u16URILen = (uint16)(((uint32)pcAux2) - ((uint32)pcAux1));

	if (sParsedRequest.eType == HTTP_POST)
	{
		pcAux1=strstr(zRequest, "{");
		if (pcAux1 == NULL)
		{
			LOG_DEBUG("Didn't find post data");
			return sParsedRequest;
		}
		pcAux2=strchr(pcAux1, '\0');

		sParsedRequest.pcPostData = pcAux1;
		sParsedRequest.u16PostLen = (uint16)(((uint32)pcAux2) - ((uint32)pcAux1));
	}
	else
	{
		sParsedRequest.pcPostData = NULL;
		sParsedRequest.u16PostLen = 0;
	}

//	LOG_DEBUG(
//		"\nRequest parsed:\n"
//		"Type: %i\n"
//		"URI Len: %u\n"
//		"Data Len: %u\n",
//		sParsedRequest.eType,
//		sParsedRequest.u16URILen,
//		sParsedRequest.u16PostLen
//	);
	return sParsedRequest;
}
