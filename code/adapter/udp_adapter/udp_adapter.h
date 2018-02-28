/**
 * @file udp_adapter.h
 * @author Lucas Petrelli
 * @date Aug 2017
 * @brief Adapters for the ESP8266 SDK's UDP feature
 *
 */
#ifndef UDP_ADAPTER_H_
#define UDP_ADAPTER_H_

#include "udp_types.h"
#include "exceptions.h"
#include "basictypes.h"
#include "mem_types.h"
#include "debug_adapter.h"

#include "esp_common.h"
#include "espconn.h"

/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 */

/**
 * @addtogroup adapter
 * @{
 */

/**
 * \defgroup udp_adapter UDP
 * @brief UDP communication API
 */

/**
 * @addtogroup udp_adapter
 * @{
 */

//#define UDPDEBUG

#ifdef UDPDEBUG
	#define LOG_UDP(msg) vUDPLog(msg)
#else
	#define LOG_UDP(msg) { do {} while(0); }
#endif


//-------------------------------------------------------------------------------------------------
/**
 * @brief     Does the preparations to work with UDP
 *
 * @param     xQueueFromUDPToRxHandle : queue that the received messages should be passed to the handler
 *
 * @return    EX_SUCCESS : operation successful
 * 		      EX_FAIL : operation failed
 */
teException eUDPInit(xQueueHandle xQueueFromUDPToRxHandle);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief     Puts a UDP message in a buffer
 *
 * @param     tsUDPMessage *psMessage : message to be buffered
 *
 * @return    EX_SUCCESS : operation successful
 * 		   EX_FAIL : operation failed
 */
teException eUDPBufferPut(tsUDPMessage *psMessage);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief     Gets a UDP message from the buffer
 *
 * @param     tsUDPMessage *psMessage : pointer to be overwritten with the message address
 *
 * @return   EX_SUCCESS : operation successful
 * 		  EX_FAIL : operation failed
 */
teException eUDPBufferGet(tsUDPMessage *psMessage);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief     Callback to be called when a UDP frame is received. Takes the data received,
 * 		   allocates a tsUDPMessage and put it in the buffer
 *
 * @param     tsUDPMessage *psMessage : pointer to be overwritten with the message address
 *
 * @return   EX_SUCCESS : operation successful
 * 		  EX_FAIL : operation failed
 */
void eUDPRecvCallback(void* psConnRx, char* pData, unsigned short u8Len);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief     Creates an UDP connection
 *
 * @param     uint8* u8Ip : IP to listen on
 * @param 	  uint32 u32Port : port to listen on
 * @param	  void (*callback)(void*, char*, uint8) : function callback to be executed when a message arrives
 *
 * @return    EX_SUCCESS : operation successful
 * 		   EX_FAIL : operation failed
 */
teException eUDPCreateListener(uint8* u8Ip, uint32 u32Port);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief     Sends data message to an UDP address
 *
 * @param     uint8* u8Ip : IP to send data on
 * @param 	  uint32 u32Port : port to send data on
 * @param	  tsUDPMessage *psMessage : UDP message to be sent
 *
 * @return    EX_SUCCESS : operation successful
 * 		   EX_FAIL : operation failed
 */
teException eUDPSendData(uint8* u8Ip, uint32 u32Port, tsUDPMessage* psMessage);
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/**
 * @brief     Creates a new UDP message
 *
 * @param	  tsUDPMessage *psBuffer : pre-allocated message structure
 * @param 	  uint8 *pu8Data: data to compose the message
 *
 * @return    EX_SUCCESS : operation successful
 * 		   EX_FAIL : operation failed
 */
teException eUDPCreateMessage(tsUDPMessage *psBuffer, uint8* pu8Data, uint32 u32Len);
//-------------------------------------------------------------------------------------------------



void vUDPLog(tsUDPMessage* psMsg);
/**
 * @}
 */

/**
 * @}
 */

#endif /* UDP_ADAPTER_H_ */
