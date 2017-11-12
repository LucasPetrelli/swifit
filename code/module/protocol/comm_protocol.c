/*
 * comm_protocol.c
 *
 *  Created on: Sep 5, 2017
 *      Author: Lucas
 */

#include "comm_protocol.h"


void vProtocolDecodeUDP(tsUDPMessage* psUDPMessage, tsProtocolMessage* psProtMessage)
{
	//TODO: add check on the message
	memcpy(&psProtMessage->u8SenderIP, psUDPMessage->u8IP_, 4);
	memcpy(&psProtMessage->u32Sequential, psUDPMessage->acData_ + SEQ_OFFSET, 4);
	memcpy(&psProtMessage->eType, psUDPMessage->acData_ + TYPE_OFFSET, 1);
	memcpy(&psProtMessage->u8DataCount, psUDPMessage->acData_ + DCOUNT_OFFSET, 1);
	memcpy(&psProtMessage->acData_, psUDPMessage->acData_ + DATA_OFFSET, psProtMessage->u8DataCount);
	memcpy(&psProtMessage->u16CRC, psUDPMessage->acData_ + DATA_OFFSET + psProtMessage->u8DataCount + 2, 2);
}

void vProtocolSendMessage(tsProtocolMessage* psProtMessage)
{

}

void vProtocolFreeMessage(tsProtocolMessage* psProtMessage)
{
	free(psProtMessage);
}
