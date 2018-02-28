/*
 * @file udp_adapter.c
 * @author Lucas Petrelli
 * @date Aug 19, 2017
 * @brief UDP adapter source file
 */
#include "udp_adapter.h"
#include "system_adapter.h"

/**
 * @brief Main UDP adapter control definition
 *
 * Definition of the structure used to aggregate information about the workings of the UDP adapter
 */
tsUDPControl sUDPControl;


teException eUDPInit(xQueueHandle xQueueFromUDPToRxHandle)
{
	teException eException = EX_SUCCESSFUL;

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
	remot_info *psRemote = NULL;
	sint8 i8Error = espconn_get_connection_info(psConn, &psRemote, 0);

	if (i8Error != 0)
	{
		printf("Error while retrieving info (Code %d)", i8Error);
		return;
	}
	tsUDPMessage* psRecvMessage = (tsUDPMessage*)zalloc(sizeof(tsUDPMessage));
	psRecvMessage->u32Len = (uint32) u8Len;
	memcpy(psRecvMessage->acData_, pData, u8Len);
	memcpy(psRecvMessage->u8IP_, psRemote->remote_ip, 4);

	if (sUDPControl.xQueueFromUDPToRxHandler != NULL)
	{
		tsMemQueueMessage *psQueueMessage = (tsMemQueueMessage *)zalloc(sizeof(tsMemQueueMessage));
		psQueueMessage->eType = UDP_MESSAGE_RX;
		psQueueMessage->pvData = (void *)psRecvMessage;
		portBASE_TYPE pxHigherPriorityTaskWoken;
		portBASE_TYPE pdResult = xQueueSendFromISR(sUDPControl.xQueueFromUDPToRxHandler,(void*) psQueueMessage, &pxHigherPriorityTaskWoken);
		free(psQueueMessage);
	}
}

teException eUDPCreateListener(uint8* u8Ip, uint32 u32Port)
{
	teException eException = EX_SUCCESSFUL;
	sint8 i8EspconnErr = 0;
	struct espconn *psConn = (struct espconn *)zalloc(sizeof(struct espconn));

	psConn->type = ESPCONN_UDP;
	psConn->state = ESPCONN_NONE;
	psConn->proto.udp = (esp_udp *)zalloc(sizeof(esp_udp));
	psConn->proto.udp->local_port = u32Port;

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
	free(psConn->proto.udp);
	free(psConn);
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

void vUDPLog(tsUDPMessage* psMsg)
{
	uint32_t u32Iterator = 0;
	LOG_DEBUG("---- UDP Message LOG ----");
	LOG_DEBUG("IP:\t%u.%u.%u.%u", psMsg->u8IP_[0], psMsg->u8IP_[1], psMsg->u8IP_[2], psMsg->u8IP_[3]);
	LOG_DEBUG("Len:\t%u", psMsg->u32Len);
	char newLine[3*16+1];
	while (u32Iterator < psMsg->u32Len)
	{
		sprintf(&newLine[(u32Iterator%16)*3],"%02x ", psMsg->acData_[u32Iterator]);
		u32Iterator += 1;
		if (u32Iterator%16 == 0)
		{
			LOG_DEBUG(newLine);
			memset(newLine, 0, 3*16+1);
		}
	}
	if (newLine[0])
	{
		LOG_DEBUG(newLine);
	}
	LOG_DEBUG("---- --------------- ----");
}
