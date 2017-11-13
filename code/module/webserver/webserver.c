/**
 * @file webserver.c
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#include "webserver.h"
#include <stdio.h>
#include <stdlib.h>

const char* zHttpHeaderOK = {
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n\r\n"
		""
};

cJSON_Hooks sJSONHooks ;

void vSetupWebserver()
{
	vHTTPSetCallback((vHTTPRequestCallback) vRequestHandler);
    xTaskCreate(&vHTTPTask, "http_server", 512, NULL, 2, NULL);
}

void vRequestHandler(char* zRequest)
{
	LOG_WEB("New request!\n%s", zRequest);
    tsHttpRequest sParsedRequest = sGetRequest(zRequest);
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

	if (strncmp(sRequest.pcURIData, URI_CGI, strlen(URI_CGI))==0)
	{
		LOG_WEB("CGI requested");
		char* p = sRequest.pcURIData + strlen(URI_CGI);
		if (strncmp(p, URI_CGI_CONFIG_WLAN, strlen(URI_CGI_CONFIG_WLAN))==0)
		{
			char* data = (char*)zalloc(sRequest.u16PostLen + 1);
			strncpy(data, sRequest.pcPostData, sRequest.u16PostLen);
			LOG_WEB("%s requested\nData: %s", URI_CGI_CONFIG_WLAN, data);


			cJSON* sObj = cJSON_Parse(data);
			if (!sObj)
			{
				LOG_WEB("Error parsing JSON ---- %s", cJSON_GetErrorPtr());
			}
			char* d = cJSON_Print(sObj);

			free(data);
			LOG_WEB("json coded then uncoded:  %s", d);

			free(d);
			cJSON_Delete(sObj);


			const char* acResponseBody = "Response ok!";
			uint32 u32ResponseLen = strlen(zHttpHeaderOK)+strlen(acResponseBody)+1;
			pcResponse = (char*)zalloc(u32ResponseLen);
			strcat(pcResponse, zHttpHeaderOK);
			strcat(pcResponse, acResponseBody);
			LOG_WEB("\n%s", pcResponse);
		}
		else
		{
			pcResponse = (char*)zalloc(strlen(acWelcomeHtml));
			strcat(pcResponse, acWelcomeHtml);
		}
	}
	else
	{
		pcResponse = (char*)zalloc(strlen(acWelcomeHtml));
		strcat(pcResponse, acWelcomeHtml);
	}

pcHandleDecodedRequest_exit:
	return pcResponse;
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

	LOG_DEBUG(
		"\nRequest parsed:\n"
		"Type: %i\n"
		"URI Len: %u\n"
		"Data Len: %u\n",
		sParsedRequest.eType,
		sParsedRequest.u16URILen,
		sParsedRequest.u16PostLen
	);
	return sParsedRequest;
}
