/*
 * @file udp_adapter.c
 * @author Lucas Petrelli
 * @date Aug 19, 2017
 * @brief UDP adapter source file
 */
#include "udp_adapter.h"


/**
 * @brief Main UDP adapter control definition
 *
 * Definition of the structure used to aggregate information about the workings of the UDP adapter
 */
tsUDPControl sUDPControl;


teException eUDPInit(xQueueHandle xQueueFromUDPToRxHandle)
{
	teException eException = EX_SUCCESSFUL;
    espconn_init();

    // Init the local message buffer
    sUDPControl.sUDPRxBuffer.u8Length = SIZE_OF_UDP_MESSAGE_BUFFER;
    sUDPControl.sUDPRxBuffer.u8Count = 0;
    sUDPControl.sUDPRxBuffer.u8Input = 0;
    sUDPControl.sUDPRxBuffer.u8Output = 0;

    sUDPControl.xQueueFromUDPToRxHandler = xQueueFromUDPToRxHandle;

    return eException;
}

teException eUDPBufferPut(tsUDPMessage *psMessage)
{
	teException eException = EX_SUCCESSFUL;

	sUDPControl.sUDPRxBuffer.psMessages[sUDPControl.sUDPRxBuffer.u8Input] = psMessage;
	sUDPControl.sUDPRxBuffer.u8Input = (sUDPControl.sUDPRxBuffer.u8Input+1)%sUDPControl.sUDPRxBuffer.u8Length;

	if (sUDPControl.sUDPRxBuffer.u8Count == sUDPControl.sUDPRxBuffer.u8Length)
	{
		// Advance the output pointer - one item is lost
		sUDPControl.sUDPRxBuffer.u8Output = (sUDPControl.sUDPRxBuffer.u8Output + 1)%sUDPControl.sUDPRxBuffer.u8Length;
	}
	else
	{
		// Increment item counter
		sUDPControl.sUDPRxBuffer.u8Count = sUDPControl.sUDPRxBuffer.u8Count + 1;
	}

    return eException;
};

teException eUDPBufferGet(tsUDPMessage *psMessage)
{
	teException eException = EX_SUCCESSFUL;
	tsUDPMessage *psMessageRetrieved = NULL;

	if (sUDPControl.sUDPRxBuffer.u8Count == 0)
	{
		SET_EXCEPTION(EX_UDP_EMPTY_BUFFER);
		return eException;
	}

	psMessageRetrieved = sUDPControl.sUDPRxBuffer.psMessages[sUDPControl.sUDPRxBuffer.u8Output];
	memcpy(psMessage, psMessageRetrieved, sizeof(tsUDPMessage));
	free(psMessageRetrieved);

	sUDPControl.sUDPRxBuffer.psMessages[sUDPControl.sUDPRxBuffer.u8Output] = NULL;
	sUDPControl.sUDPRxBuffer.u8Output = (sUDPControl.sUDPRxBuffer.u8Output+1)%sUDPControl.sUDPRxBuffer.u8Length;
	sUDPControl.sUDPRxBuffer.u8Count = sUDPControl.sUDPRxBuffer.u8Count - 1;

    return eException;
}

void eUDPRecvCallback(void* psConnRx, char* pData, unsigned short u8Len)
{
	struct espconn *psConn = (struct espconn *)psConnRx;
	remot_info *sRemote = (remot_info*)zalloc(sizeof(remot_info));
	sint8 i8Error = espconn_get_connection_info(psConn, &sRemote, 0);

	if (i8Error != 0)
	{
		printf("Error while retrieving info (Code %d)", i8Error);
		return;
	}

	tsUDPMessage* psRecvMessage = (tsUDPMessage*)zalloc(sizeof(tsUDPMessage));
	psRecvMessage->u32Len = (uint32) u8Len;
	memcpy(psRecvMessage->acData_, pData, u8Len);
	memcpy(psRecvMessage->u8IP_, sRemote->remote_ip, 4);

	LOG_DEBUG("UDP message created\n")

	if (sUDPControl.xQueueFromUDPToRxHandler != NULL)
	{
		tsMemQueueMessage *psQueueMessage = (tsMemQueueMessage *)zalloc(sizeof(tsMemQueueMessage));
		psQueueMessage->eType = UDP_MESSAGE_RX;
		psQueueMessage->pvData = (void *)psRecvMessage;
		portBASE_TYPE pxHigherPriorityTaskWoken;
		portBASE_TYPE pdResult = xQueueSendFromISR(sUDPControl.xQueueFromUDPToRxHandler,(void*) psQueueMessage, &pxHigherPriorityTaskWoken);

		#ifdef DEBUG
			if (pdResult == pdTRUE)
			{
				printf("UDP message passed to queue!\n");
			}
			else
			{
				printf("UDP queue full!\n");
			}
		#endif
	}

	eUDPBufferPut(psRecvMessage);
	LOG_DEBUG("UDP message buffered\n");
}

teException eUDPCreateListener(uint8* u8Ip, uint32 u32Port)
{
	teException eException = EX_SUCCESSFUL;
	sint8 i8EspconnErr = 0;
	struct espconn *psConn = (struct espconn *)zalloc(sizeof(struct espconn));

	psConn->type = ESPCONN_UDP;
	psConn->state = ESPCONN_NONE;
	psConn->proto.udp = (esp_udp *)zalloc(sizeof(esp_udp));
	psConn->proto.udp->local_port = 1000;

	memcpy(psConn->proto.udp->local_ip, u8Ip, 4);

	i8EspconnErr = espconn_regist_recvcb(psConn, eUDPRecvCallback);
	if (i8EspconnErr != 0)
	{

		LOG_DEBUG("Error registering receive callback (Code %d)", i8EspconnErr);
		teException eException = EX_GENERIC_FAIL;
		goto out_UDPCreateListener;
	}

	i8EspconnErr = espconn_create(psConn);
	if (i8EspconnErr != 0)
	{
		LOG_DEBUG("Error while creating connection (Code %d)", i8EspconnErr);
		SET_EXCEPTION(EX_GENERIC_FAIL);
		goto out_UDPCreateListener;
	}

	LOG_DEBUG(
		"Will start listening on  %u . %u . %u . %u\n"
		"Port %u\n",
		psConn->proto.udp->local_ip[0],
		psConn->proto.udp->local_ip[1],
		psConn->proto.udp->local_ip[2],
		psConn->proto.udp->local_ip[3],
		psConn->proto.udp->local_port
	);
	sUDPControl.psConnListener = psConn;

out_UDPCreateListener:
	return eException;
}

teException eUDPSendData(uint8* u8Ip, uint32 u32Port, tsUDPMessage* psMessage)
{
	teException eException = EX_SUCCESSFUL;
	struct espconn *psConn = (struct espconn *)zalloc(sizeof(struct espconn));

	psConn->type = ESPCONN_UDP;
	psConn->state = ESPCONN_NONE;
	psConn->proto.udp = (esp_udp *)zalloc(sizeof(esp_udp));
	memcpy(psConn->proto.udp->remote_ip, u8Ip, 4);
	psConn->proto.udp->remote_port = u32Port;

	espconn_create(psConn);
	espconn_sendto(psConn, psMessage->acData_, psMessage->u32Len);
out_UDPSendData:
	espconn_delete(psConn);
	return eException;
}

teException eUDPCreateMessage(tsUDPMessage *psBuffer, uint8* pu8Data, uint32 u32Len)
{
	teException eException = EX_SUCCESSFUL;

	psBuffer->u32Len = u32Len;
	memcpy(psBuffer->acData_, pu8Data, u32Len);

	if (bWifiIsWifiConnected())
	{
		eWifiRetrieveCurrentIP(psBuffer->u8IP_);
	}

	return eException;
}
