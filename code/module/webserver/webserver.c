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
#include "cJSON.h"

cJSON_Hooks sJSONHooks ;

void vSetupWebserver()
{
	// Set the callback that handles an received HTTP request
	vHTTPSetCallback((vHTTPRequestCallback) vRequestHandler);

	// Launch the HTTP listener task
	// It calls the callback configured above when an HTTP request is received
    xTaskCreate(&vHTTPTask, "http_server", 512, NULL, 4, NULL);
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
		LOG_WEB("CGI requested");
		char* p = sRequest.pcURIData + strlen(URI_CGI);
		char* zCGI = (char*)zalloc(sRequest.u16URILen - strlen(URI_CGI) + 1);
		char* data = (char*)zalloc(sRequest.u16PostLen + 1);
		strncpy(zCGI, p, sRequest.u16URILen - strlen(URI_CGI));
		strncpy(data, sRequest.pcPostData, sRequest.u16PostLen);

		LOG_WEB("%s requested\nData: %s", zCGI, data);
		if (strncmp(p, zConfigWlanCGI, strlen(zConfigWlanCGI))==0)
		{
			pcResponse = zHandlerConfigWlan(data);
		}
		else if (strncmp(p, zStatusCGI, strlen(zStatusCGI))==0)
		{
			pcResponse = zHandlerStatus(data);
		}
		else if (strncmp(p, zConfigMessageCGI, strlen(zConfigMessageCGI))==0)
		{
			pcResponse = zHandlerConfigMessage(data);
		}
		else if (strncmp(p, zDeviceRestartCGI, strlen(zDeviceRestartCGI))==0)
		{
			pcResponse = zHandlerDeviceRestart(data);
		}
		else if (strncmp(p, zDeviceListCGI, strlen(zDeviceListCGI))==0)
		{
			pcResponse = zHandlerDeviceList(data);
		}
		else if (strncmp(p, zIssueActionCGI, strlen(zIssueActionCGI))==0)
		{
			pcResponse = zHandlerIssueAction(data);
		}

		free(zCGI);
		free(data);
	}

pcHandleDecodedRequest_exit:
	return pcResponse;
}

char* zReadHomePageFromFlash(char* address)
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
		pcHomepage = zReadHomePageFromFlash((char*) acWelcome_configHtml);
	}
	else
	{
		pcHomepage = zReadHomePageFromFlash((char*) acHomepageHtml);
	}
	return pcHomepage;
}

cJSON* psWebserverMakeJSONFromDevice(tsDevice* psDev)
{
	// create object to represent self
	cJSON* psJSONSelf = cJSON_CreateObject();

	// create itens for represent self information
	char cId_[11];
	sprintf(cId_, "%u", psDev->u32ID);

	cJSON_AddStringToObject(psJSONSelf, "name", psDev->cName_);
	cJSON_AddStringToObject(psJSONSelf, "id", cId_);
	if (psDev->eType == DEVICE_SWITCH)
	{
		cJSON_AddStringToObject(psJSONSelf, "type", "Switch");
		cJSON_AddFalseToObject(psJSONSelf, "hasPir");
	}
	else if (psDev->eType == DEVICE_SOCKET)
	{
		cJSON_AddStringToObject(psJSONSelf, "type", "Plug");
		cJSON_AddTrueToObject(psJSONSelf, "hasPir");

		teSensorState ePIRState = psDev->eSensorState;
		if (ePIRState == TRIGGERED)
		{
			cJSON_AddTrueToObject(psJSONSelf, "pirStatus");
		}
		else
		{
			cJSON_AddFalseToObject(psJSONSelf, "pirStatus");
		}

		teActuatorState eRelayState = psDev->eActuatorState;
		if (eRelayState == ACTIVATED)
		{
			cJSON_AddTrueToObject(psJSONSelf, "relayStatus");
		}
		else
		{
			cJSON_AddFalseToObject(psJSONSelf, "relayStatus");
		}

	}
	return psJSONSelf;
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
		pcAux1=strstr(zRequest, "\r\n\r\n");
		if (pcAux1 == NULL)
		{
			LOG_DEBUG("Didn't find post data");
			return sParsedRequest;
		}
		pcAux1 =(char*)((uint32)pcAux1 + 4);
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
