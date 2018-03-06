/*
 * comm_protocol.c
 *
 *  Created on: Sep 5, 2017
 *      Author: Lucas
 */

#include "comm_protocol.h"
#include "webserver.h"
#include "devices.h"

void vProtocolInit()
{
	uint8 pu8LocalIP[4];
    eWifiRetrieveCurrentIP(pu8LocalIP);
    eUDPCreateListener(pu8LocalIP, PROT_PORT);
}

void vProtocolDecodeUDP(tsUDPMessage* psUDPMessage, tsProtocolMessage* psProtMessage)
{
	//TODO: add check on the message
	memcpy(psProtMessage->u8IP_, psUDPMessage->u8IP_, 4);
	memcpy(&psProtMessage->u32Sequential, psUDPMessage->acData_ + SEQ_OFFSET, 4);
	memcpy(&psProtMessage->eType, psUDPMessage->acData_ + TYPE_OFFSET, 1);
	memcpy(&psProtMessage->u32DataCount, psUDPMessage->acData_ + DCOUNT_OFFSET, 1);
	memcpy(psProtMessage->acData_, psUDPMessage->acData_ + DATA_OFFSET, psProtMessage->u32DataCount);
}

void vProtocolSendMessage(tsProtocolMessage* psProtMessage)
{
	tsUDPMessage* psUDPMessage = (tsUDPMessage*)zalloc(sizeof(tsUDPMessage));

	psProtMessage->u32Sequential = 0x00000000;
	memcpy(psUDPMessage->acData_ + SEQ_OFFSET, &psProtMessage->u32Sequential, 4);
	memcpy(psUDPMessage->acData_ + TYPE_OFFSET, &psProtMessage->eType, 1);
	memcpy(psUDPMessage->acData_ + DCOUNT_OFFSET, &psProtMessage->u32DataCount, 4);
	memcpy(psUDPMessage->acData_ + DATA_OFFSET, psProtMessage->acData_, psProtMessage->u32DataCount);
	memcpy(psUDPMessage->u8IP_, psProtMessage->u8IP_, 4);
	psUDPMessage->u32Len = 4 + 1 + 4 + psProtMessage->u32DataCount;
	eUDPSendData(psProtMessage->u8IP_, PROT_PORT, psUDPMessage);
	free(psUDPMessage);
}

void vProtocolFreeMessage(tsProtocolMessage* psProtMessage)
{
	free(psProtMessage);
}

void vProtocolDoBroadcast()
{
	tsProtocolMessage* psMsg = psProtocolMakeBroadcastMsg();
	vProtocolSendMessage(psMsg);
	free(psMsg);
}

tsProtocolMessage* psProtocolMakeBroadcastMsg()
{
	tsProtocolMessage* psMsg = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
	psMsg->eType = MSG_BROADCAST;
	psMsg->u8IP_[0] = 192;
	psMsg->u8IP_[1] = 168;
	psMsg->u8IP_[2] = 0;
	psMsg->u8IP_[3] = 255;

	LOG_DEBUG("Making broadcast");
	psMsg->u32DataCount = 0;
	return psMsg;
}

tsProtocolMessage* psProtocolMakeStatus(uint8_t* pu8TargetIp)
{
	tsProtocolMessage* psMsg = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
	psMsg->eType = MSG_STATUS;
	memcpy(psMsg->u8IP_, pu8TargetIp, 4);

	LOG_DEBUG("Making status");
	tsDevice* psSelfInfo = psDeviceGetSelf();
	LOG_DEV(psSelfInfo);
	memcpy(psMsg->acData_, (void*)(((uint8_t*)psSelfInfo)+4), sizeof(tsDevice)-4);
	psMsg->u32DataCount = sizeof(tsDevice)-4;
	free(psSelfInfo);
	return psMsg;
}

tsProtocolMessage* psProtocolMakeAction(tsActuatorTaskRequest* psReq, uint8_t* pu8TargetIp)
{
	tsProtocolMessage* psMsg = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
	LOG_DEBUG("Making action");

	psMsg->eType = MSG_ACTION;
	memcpy(psMsg->u8IP_, pu8TargetIp, 4);
	memcpy(psMsg->acData_, (void*) psReq, sizeof(tsActuatorTaskRequest));
	psMsg->u32DataCount = sizeof(tsActuatorTaskRequest);
	return psMsg;
}

tsProtocolMessage* psProtocolMakeTimingParameter(tsDeviceTimingChange* psReq, uint8_t* pu8TargetIp)
{
	tsProtocolMessage* psMsg = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
	LOG_DEBUG("Making timing parameter");

	psMsg->eType = MSG_TIMING_PARAMETER;
	memcpy(psMsg->u8IP_, pu8TargetIp, 4);
	memcpy(psMsg->acData_, (void*) psReq, sizeof(tsDeviceTimingChange));
	psMsg->u32DataCount = sizeof(tsDeviceTimingChange);
	return psMsg;
}

tsProtocolMessage* psProtocolMakeEvent(tsNetworkEvent* psEvent, uint8_t* pu8TargetIp)
{
	tsProtocolMessage* psMsg = (tsProtocolMessage*)zalloc(sizeof(tsProtocolMessage));
	psMsg->eType = MSG_EVENT;
	memcpy(psMsg->u8IP_, pu8TargetIp, 4);
	memcpy(psMsg->acData_, (void*) psEvent, sizeof(tsNetworkEvent));
	psMsg->u32DataCount = sizeof(tsNetworkEvent);
	return psMsg;

}

void vProtocolLog(tsProtocolMessage* psMsg)
{
	LOG_DEBUG("---- Protocol Message LOG ----");
	LOG_DEBUG("Message Type:\t%u", psMsg->eType);
	LOG_DEBUG("Data count:\t%u", psMsg->u32DataCount);

	uint32_t u32Iterator = 0;
	char newLine[3*16+1];

	while (u32Iterator < psMsg->u32DataCount)
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

	LOG_DEBUG("---- Protocol Message LOG ----");
}
