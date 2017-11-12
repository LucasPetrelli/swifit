/**
 * @file udp_types.h
 * @author Lucas
 * @date Oct 8, 2017
 *
 */

#ifndef UDP_TYPES_H_
#define UDP_TYPES_H_
#include "basictypes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/portmacro.h"

/**
 * @brief UDP message structure
 *
 * Structure to unite an UDP message data and information
 */
typedef struct {
	uint32 u32Len;				/**< The valid length of the data field */
	uint8 u8IP_[4];				/**< The IP assigned to the message (sender when msg received, self when msg tx) */
	char acData_[256];			/**< Buffer for the message data */
} tsUDPMessage;

/**
 * @brief Defines the size of the UDP message buffer
 */
#define SIZE_OF_UDP_MESSAGE_BUFFER (uint8)16

/**
 * @brief UDP message buffer definition
 *
 * Buffer to hold received UDP messages when FreeRTOS queue is not to be used
 */
typedef struct {
	uint8 u8Length;				/**< Length to the buffer */
	uint8 u8Count;				/**< Count of items in the buffer */
	uint32 u8Input; 			/**< Pointer to the input element (will receive next put) */
	uint32 u8Output;			/**< Pointer to the output element (will be popped on next get) */

	tsUDPMessage* psMessages[SIZE_OF_UDP_MESSAGE_BUFFER]; /**< Buffer of messages */
} tsUDPBuffer;


typedef struct {
	tsUDPBuffer sUDPRxBuffer;

	struct espconn *psConnListener;
	xQueueHandle xQueueFromUDPToRxHandler;
} tsUDPControl;

#endif /* UDP_TYPES_H_ */
