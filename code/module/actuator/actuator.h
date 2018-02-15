/*
 * actuator.h
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_ACTUATOR_ACTUATOR_H_
#define CODE_MODULE_ACTUATOR_ACTUATOR_H_

#include "actuator_types.h"

#define MAX_ACTUATOR 3

void vActuatorInitPlatform();
void vActuatorActivate(tsActuator* psActuator);
void vActuatorDeactivate(tsActuator* psActuator);
void vActuatorActivateById(teActuatorId eId);
void vActuatorDeactivateById(teActuatorId eId);
void vActuatorToggleById(teActuatorId eId);




#endif /* CODE_MODULE_ACTUATOR_ACTUATOR_H_ */
