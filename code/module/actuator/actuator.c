/*
 * actuator.c
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */


#include "actuator.h"
#include "configuration.h"

/**
 * @brief Actuator control structure
 *
 * Allocates one structure for each available allocator. This list can be indexed by actuator IDs (teActuatorId)
 * Only available locally
 */
tsActuator asActuator[MAX_ACTUATOR];

void vActuatorInitPlatform()
{
	// Call to the GPIO muxes, selecting GPIO as output on the given pins
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);

	// Configuration of red LED
	asActuator[LED_RED].eId = LED_RED;
	asActuator[LED_RED].eType = LED;
	asActuator[LED_RED].eGPIOAssigned = GPIO_12;
	asActuator[LED_RED].eState = DEACTIVATED;

	vGPIOSetIOMode(GPIO_BIT(asActuator[LED_RED].eGPIOAssigned), GPIO_Output);
	vActuatorDeactivate(&asActuator[LED_RED]);

	// Configuration of blue LED
	asActuator[LED_BLUE].eId = LED_BLUE;
	asActuator[LED_BLUE].eType = LED;
	asActuator[LED_BLUE].eGPIOAssigned = GPIO_14;
	asActuator[LED_BLUE].eState = DEACTIVATED;

	vGPIOSetIOMode(GPIO_BIT(asActuator[LED_BLUE].eGPIOAssigned), GPIO_Output);
	vActuatorDeactivate(&asActuator[LED_BLUE]);


	// Configuration of the relay
	asActuator[MAIN_RELAY].eId = MAIN_RELAY;
	asActuator[MAIN_RELAY].eType = RELAY;
	asActuator[MAIN_RELAY].eGPIOAssigned = GPIO_4;
	vGPIOSetIOMode(GPIO_BIT(asActuator[MAIN_RELAY].eGPIOAssigned), GPIO_Output);
	teActuatorState eRelayInitialState = (teActuatorState) u8ConfigurationGetActuatorState();
	if (eRelayInitialState == ACTIVATED)
	{
		vActuatorActivate(&asActuator[MAIN_RELAY]);
	}
	else
	{
		vActuatorDeactivate(&asActuator[MAIN_RELAY]);
	}

}

void vActuatorActivate(tsActuator* psActuator)
{
	vGPIOWrite(GPIO_BIT(psActuator->eGPIOAssigned), HIGH);
	psActuator->eState = ACTIVATED;
}

void vActuatorDeactivate(tsActuator* psActuator)
{
	vGPIOWrite(GPIO_BIT(psActuator->eGPIOAssigned), LOW);
	psActuator->eState = DEACTIVATED;
}


void vActuatorActivateById(teActuatorId eId)
{
	vActuatorActivate(&asActuator[eId]);
	if (eId == MAIN_RELAY)
	{
		vConfigurationSetActuatorState((uint8_t)ACTIVATED);
	}
}

void vActuatorDeactivateById(teActuatorId eId)
{
	vActuatorDeactivate(&asActuator[eId]);
	if (eId == MAIN_RELAY)
	{
		vConfigurationSetActuatorState((uint8_t)DEACTIVATED);
	}
}

void vActuatorToggleById(teActuatorId eId)
{
	if (asActuator[eId].eState == ACTIVATED)
	{
		vActuatorDeactivate(&asActuator[eId]);
	}
	else
	{
		vActuatorActivate(&asActuator[eId]);
	}
}

teActuatorState eActuatorGetStateById(teActuatorId eId)
{
	return asActuator[eId].eState;
}
