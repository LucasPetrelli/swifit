/*
 * configuration.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_CONFIGURATION_CONFIGURATION_H_
#define CODE_MODULE_CONFIGURATION_CONFIGURATION_H_

#include "configuration_types.h"

void vConfigurationRead();
void vConfigurationSetDefault();
void vConfigurationSet(tsConfiguration* psConfig);
tsConfiguration* psConfigurationGet();
void vConfigurationClearMessage();


#endif /* CODE_MODULE_CONFIGURATION_CONFIGURATION_H_ */
