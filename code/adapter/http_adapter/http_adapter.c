/**
 * @file http_adapter.c
 * @author Lucas
 * @date Oct 31, 2017
 *
 * @brief
 */
#include "http_adapter.h"

tsHTTPControl sHTTPControl;

void vHTTPTask(void *pvParameters)
{
	// Initialize control structure
	sHTTPControl.client = NULL;
	sHTTPControl.server = netconn_new(NETCONN_TCP);

	LOG_DEBUG("HTTP task begin");

	// If HTTP tcp socket weren't allocated, delete the task
	if (sHTTPControl.server == NULL)
	{
		LOG_HTTP("Failed to allocate http server socket!");
		vTaskDelete(NULL);
	}

	// Binds the socket to the HTTP port 80 and start listening
    netconn_bind(sHTTPControl.server, IP_ADDR_ANY, 80);
    netconn_listen(sHTTPControl.server);
    LOG_DEBUG("Listening...");
    while (1)
    {
    	// Wait for a connection
    	err_t eErr = netconn_accept(sHTTPControl.server, &sHTTPControl.client);
    	netconn_set_sendtimeout(sHTTPControl.client, 100);
    	LOG_DEBUG("Got HTTP connection!");
    	if (eErr == ERR_OK)
    	{
    		// Read the incoming data
    		struct netbuf* sNetBuffer;
    		eErr = netconn_recv(sHTTPControl.client, &sNetBuffer);
        	LOG_DEBUG("Read data [%u]!", eErr);
    		if (eErr == ERR_OK)
    		{
    			void* vData;
    			uint16 u16Len;
    			netbuf_data(sNetBuffer, &vData, &u16Len);
            	LOG_DEBUG("Passing to callback [%u]!", u16Len);
    			sHTTPControl.vCallback((char*) vData);
            	LOG_DEBUG("Callback done!", u16Len);
    		}
        	netbuf_delete(sNetBuffer);
    	}
        netconn_close(sHTTPControl.client);
        netconn_delete(sHTTPControl.client);
    }

}

void vHTTPSetCallback(vHTTPRequestCallback vFunc)
{
	sHTTPControl.vCallback = vFunc;
}

void vHTTPSendAnswer(char* zAnswer)
{
	LOG_DEBUG("Sending answer! (%u b)", strlen(zAnswer));
	netconn_set_sendtimeout(sHTTPControl.client, 1000);
	s8_t s8Err = netconn_write(sHTTPControl.client, zAnswer, strlen(zAnswer), NETCONN_COPY );
	LOG_DEBUG("answer done! [%i]", s8Err);
}

