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


cJSON_Hooks sJSONHooks ;

void vSetupWebserver()
{
	// Set the callback that handles an received HTTP request
	vHTTPSetCallback((vHTTPRequestCallback) vRequestHandler);

	// Launch the HTTP listener task
	// It calls the callback configured above when an HTTP request is received
    xTaskCreate(&vHTTPTask, "http_server", 512, NULL, 2, NULL);
}

void vRequestHandler(char* zRequest)
{
	LOG_WEB("New request!\n%s", zRequest);

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
		free(zCGI);
		free(data);
	}

pcHandleDecodedRequest_exit:
	return pcResponse;
}

char* zGetHomepage()
{
	char* pcHomepage = NULL;
	tsConfiguration* psDeviceConfig = psConfigurationGet();
	if (psDeviceConfig->eMode == CONFIG_MODE)
	{
		pcHomepage = (char*)zalloc(strlen(acWelcome_configHtml));
		strcat(pcHomepage, acWelcome_configHtml);
	}
	else
	{
		// Using the config webpage as placeholder
		pcHomepage = (char*)zalloc(strlen(acWelcome_configHtml));
		strcat(pcHomepage, acWelcome_configHtml);
	}

	return pcHomepage;
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
