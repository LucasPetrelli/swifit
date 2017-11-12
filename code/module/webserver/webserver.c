/**
 * @file webserver.c
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */

#include "welcome.html.h"
#include "webserver.h"


void vSetupWebserver()
{
	vHTTPSetCallback((vHTTPRequestCallback) vRequestHandler);
    xTaskCreate(&vHTTPTask, "http_server", 2048, NULL, 2, NULL);

}

void vRequestHandler(char* zRequest)
{
	LOG_DEBUG("New request!\n%s", zRequest);
    const char *webpage = acWelcomeHtml;
    tsHttpRequest sParsedRequest = sGetRequest(zRequest);
	vHTTPSendAnswer((char*) acWelcomeHtml);
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
	LOG_DEBUG("pcAux1 = %u", (uint32)pcAux1);


	pcAux2=strchr(pcAux1, ' ');
	LOG_DEBUG("pcAux2 = %u", (uint32)pcAux2);
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
		LOG_DEBUG("pcAux1 = %u", (uint32)pcAux1);
		LOG_DEBUG("pcAux2 = %u", (uint32)pcAux2);

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
