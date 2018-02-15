/*
 * configuration_types.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_CONFIGURATION_CONFIGURATION_TYPES_H_
#define CODE_MODULE_CONFIGURATION_CONFIGURATION_TYPES_H_


/*
 * @brief Device operation type
 */
typedef enum {
	CONFIG_MODE = '0',				/**< Device in configuration mode */
	NORMAL_MODE = '1',				/**< Device in normal operation mode */
	ATTEMPT_NEW_CONNECTION = '2',	/**< Device trying to connect to a new AP given in CONFIG_MODE */
	CONNECTING_TO_KNOWN_AP = '3',   /**< Device trying to connect to an AP previously verified */
} teOperationMode;


typedef struct {
	char acAP[32];
	char acPassword[32];
	char acMessage[64];
	teOperationMode eMode;
} tsConfiguration;


#endif /* CODE_MODULE_CONFIGURATION_CONFIGURATION_TYPES_H_ */
