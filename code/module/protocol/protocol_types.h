
#include "basictypes.h"
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
 * \defgroup comm_prot_types Types
 * @brief Communication Protocol type definitions
 * @addtogroup comm_prot_types
 * @{
 */

#define SEQ_OFFSET 0		//!< Offset where the Sequential field starts
#define TYPE_OFFSET 4		//!< Offset where the Type field starts
#define DCOUNT_OFFSET 5		//!< Offset where the Data Count field starts
#define DATA_OFFSET 9		//!< Offset where the Data field starts

#define PROT_PORT 2345		//!< The UDP port used by the devices
/**
 * @brief Message type enumerator
 *
 * Enumeration used to distinguish message types. Used to handle data accordingly to the message type
 */
typedef enum {
	MSG_BROADCAST,							/**< Broadcast message type */
	MSG_STATUS,								/**< Status message type */
	MSG_EVENT,								/**< Event message type */
	MSG_ACTION,								/**< Action message type */
	MSG_TIMING_PARAMETER,					/**< Parameter message type */
	MSG_TYPE_N								/**< Message type count */
} teProtocolMessageType;

/**
 * @brief Protocol message structure
 *
 * Contains the data that represents a protocol message, separated with labels
 */
typedef struct {
	uint32_t u32Sequential;					/**< Message sequential number */
	teProtocolMessageType eType;			/**< Message type */
	uint8_t u8IP_[4];						/**< IP from the message sender */
	uint32_t u32DataCount;					/**< Amount of bytes in the buffer pointed by pu8Data */
	uint8_t acData_[128];					/**< Pointer to the message data buffer */
} tsProtocolMessage;

/**
 * @}
 * @}
 * @}
 */
