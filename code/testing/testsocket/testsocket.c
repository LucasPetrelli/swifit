#include "testsocket.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_adapter.h"
#include "udp_adapter.h"


void vTestSocketRxTask(void* args)
{
	// Error propagation
	sint8 i8Error = 0;
	uint8 pu8LocalIP[4];
	uint32 u32Port = 1000;

    printf("Waiting to get IP\n");
    eWifiWaitToBeConnected();
    eWifiRetrieveCurrentIP(pu8LocalIP);
    eUDPCreateListener(pu8LocalIP, u32Port);

	while(1)
	{
		tsMemQueueMessage *psQueueMesssage = (tsMemQueueMessage *)zalloc(sizeof(tsMemQueueMessage));
		tsUDPMessage *psMessage = (tsUDPMessage *)zalloc(sizeof(tsUDPMessage));
		portBASE_TYPE pdResult = xQueueReceive(sUDPControl.xQueueFromUDPToRxHandler,(const void*) psQueueMesssage, portMAX_DELAY);



		if (pdResult == errQUEUE_EMPTY)
		{
			printf(".\n");
			continue;
		}
		printf("Popped from queue successfully\n");
		psMessage = (tsUDPMessage *) psQueueMesssage->pvData;

		printf(
			"Received from %u . %u . %u . %u\n"
			"Data: %s\n",
			psMessage->u8IP_[0],
			psMessage->u8IP_[1],
			psMessage->u8IP_[2],
			psMessage->u8IP_[3],
			psMessage->acData_
		);

		tsUDPMessage *psReply = (tsUDPMessage *)zalloc(sizeof(tsUDPMessage));
		char* pcMessage = "I received something!";
		uint32 u32MsgLen = strlen(pcMessage);

		eUDPCreateMessage(psReply, pcMessage, u32MsgLen);
		eUDPSendData(psMessage->u8IP_, 58773, psReply);

		free(psMessage);
		free(psReply);
	}
}
