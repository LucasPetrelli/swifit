#define STX_OFFSET 0
#define SEQ_OFFSET 1
#define TYPE_OFFSET 5
#define DCOUNT_OFFSET 6
#define OPEN_OFFSET 7
#define DATA_OFFSET 8

#include "basictypes.h"

/**
 * @brief Message type enumerator
 *
 * Enumeration used to distinguish message types. Used to handle data accordingly to the message type
 */
typedef enum {
	BROADCAST,							/**< Broadcast message type */
	STATUS,								/**< Status message type */
	EVENT,								/**< Event message type */
	ACTION,								/**< Action message type */
	PARAMETER,							/**< Parameter message type */
	MSG_TYPE_N							/**< Message type count */
} teProtocolMessageType;

/**
 * @brief Protocol message structure
 *
 * Contains the data that represents a protocol message, separated with labels
 */
typedef struct {
	uint32 u32Sequential;				/**< Message sequential number */
	teProtocolMessageType eType;		/**< Message type */
	uint8 u8SenderIP[4];				/**< IP from the message sender */
	uint8 u8DataCount;					/**< Amount of bytes in the buffer pointed by pu8Data */
	uint8 acData_[246];					/**< Pointer to the message data buffer */
	uint16 u16CRC;						/**< CRC of the message */
} tsProtocolMessage;
