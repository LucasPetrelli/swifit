/*
 * comm_protocol.h
 *
 *  Created on: Sep 5, 2017
 *      Author: Lucas
 */

#ifndef CODE_MODULE_PROTOCOL_COMM_PROTOCOL_H_
#define CODE_MODULE_PROTOCOL_COMM_PROTOCOL_H_

#include "protocol_types.h"
#include "udp_types.h"

void vProtocolDecodeUDP(tsUDPMessage* psUDPMessage, tsProtocolMessage* psProtMessage);
void vProtocolSendMessage(tsProtocolMessage* psProtMessage);
void vProtocolFreeMessage(tsProtocolMessage* psProtMessage);


#endif /* CODE_MODULE_PROTOCOL_COMM_PROTOCOL_H_ */
