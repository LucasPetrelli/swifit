/**
 * @file mem_types.h
 * @author Lucas
 * @date Oct 8, 2017
 *
 */


#ifndef CODE_MODULE_MEMORY_MEM_TYPES_H_
#define CODE_MODULE_MEMORY_MEM_TYPES_H_
/**
 * \addtogroup adapter
 * @{
 */

/**
 * \addtogroup types
 * @{
 */


/**
 * \defgroup memory Memory
 * @brief definition of basic memory data types (e.g. Queue types)
 */

/**
 * \addtogroup memory
 * @{
 */



/**
 * @brief Queue message type enumeration
 *
 * Enumerates the type of the queue message, so that it can be decoded accordingly
 */
typedef enum {
	UDP_MESSAGE_RX,			/**< Received UDP message queue item */
	SENSOR_MESSAGE,			/**< Sensor status message */
	ACTUATOR_MESSAGE,		/**< Actuator request message */
	RESTART_MESSAGE,		/**< Device restart messa */
	N_QUEUE_MSG_TYPES,		/**< Queue item types count */
} teMemQueueMessageType;

/**
 * @brief Queue message structure
 *
 * Queue message structure to pass data around using the FreeRTOS queue
 */
typedef struct {
	teMemQueueMessageType eType;	/**< Queue item type */
	void* pvData;					/**< Queue item data */
} tsMemQueueMessage;

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
#endif /* CODE_MODULE_MEMORY_MEM_TYPES_H_ */
