/*
 * comm_protocol.h
 *
 *  Created on: Sep 5, 2017
 *      Author: Lucas
 */

#ifndef CODE_MODULE_PROTOCOL_COMM_PROTOCOL_H_
#define CODE_MODULE_PROTOCOL_COMM_PROTOCOL_H_

// Type definitions
#include "protocol_types.h"

// Directly used adapters
#include "udp_adapter.h"

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 *
 * \defgroup comm_prot Communication Protocol
 * @brief Handling of data sent and received by the UDP interface
 * @{
 *
 * \defgroup comm_prot_api API
 * @brief Communication Protocol functions
 * @addtogroup comm_prot_api
 * @{
 */



#define PROTDEBUG

#ifdef PROTDEBUG
	#define LOG_PROT(msg) vProtocolLog(msg)
#else
	#define LOG_PROT(msg) {do {} while(0); }
#endif

/**
 * @brief Initialization of the protocol.
 *
 * Initializes the UDP listener. Must be called only after the wifi got connection to the AP.
 */
void vProtocolInit();

/**
 * @brief Decodes an received UDP message into a Protocol message, with information fields separated.
 * @param psUDPMessage pointer to the received UDP message.
 * @param psProtMessage pointer to a pre-allocated Protocol message structure.
 */
void vProtocolDecodeUDP(tsUDPMessage* psUDPMessage, tsProtocolMessage* psProtMessage);

/**
 * @brief Sends a protocol message.
 * @param psProtMessage the message to be sent.
 *
 * The message must have the fields with valid information.
 * The message is sent to the IP specified in the psProtMessage structure.
 */
void vProtocolSendMessage(tsProtocolMessage* psProtMessage);

/**
 * @brief Deletes a protocol message.
 * @param psProtMessage the message to be deleted.
 *
 * Calls free on the message pointer. May implement further actions in the future, if needed, so use this instead of
 * free directly to maintain consistency throughout versions.
 */
void vProtocolFreeMessage(tsProtocolMessage* psProtMessage);

/**
 * @brief Sends a broadcast message to the current /24 subnetwork.
 *
 * Creates, sends and deletes the broadcast message. No further actions are needed by the caller.
 */
void vProtocolDoBroadcast();

/**
 * @brief Allocates and populates a new broadcast message.
 * @return Pointer to the created message.
 *
 * The caller is responsible of calling vProtocolFreeMessage into the returned appointed structure.
 */
tsProtocolMessage* psProtocolMakeBroadcastMsg();

/**
 * @brief Allocates and populates a new status message.
 * @param pu8TargetIp The target of the message.
 * @return Pointer to the created message.
 *
 * The caller is responsible of calling vProtocolFreeMessage into the returned appointed structure.
 */
tsProtocolMessage* psProtocolMakeStatus(uint8_t* pu8TargetIp);

/**
 * @brief Logs using LOG_DEBUG the message data in a "beautiful" manner
 * @param psMsg The message to be logged
 */
void vProtocolLog(tsProtocolMessage* psMsg);

/**
 * @}
 * @}
 * @}
 */
#endif /* CODE_MODULE_PROTOCOL_COMM_PROTOCOL_H_ */
