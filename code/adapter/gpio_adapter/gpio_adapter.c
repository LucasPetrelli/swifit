/**
 * @file gpio_adapter.c
 * @author Lucas
 * @date Oct 9, 2017
 *
 * @brief ESP8266 GPIO adapter source code
 */

#include "gpio_adapter.h"

void vGPIOSetIOMode(uint16 u16GPIOBitfield, teGPIOMode eMode)
{
	if (eMode == GPIO_Input)
	{
		LOG_DEBUG("Setting %x as %u", u16GPIOBitfield, eMode);
		GPIO_AS_INPUT(u16GPIOBitfield);
	} else
	{
		GPIO_AS_OUTPUT(u16GPIOBitfield);
	}
}

teGPIOValue eGPIORead(teGPIONumber eNumber)
{
	return (teGPIOValue) GPIO_INPUT_GET((uint32 ) eNumber);
}

void vGPIOWrite(teGPIOBit eGPIOGroup, teGPIOValue eValue)
{
	GPIO_OUTPUT((uint16 ) eGPIOGroup, (uint16 ) eValue);
}

void vGPIOIntrSetTrigger(teGPIONumber eNumber, teGPIOIntrMode eMode)
{
	gpio_pin_intr_state_set((uint32) eNumber, (GPIO_INT_TYPE) eMode);
	//clear interrupt status
	//GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(eNumber));
}

void vGPIOIntrSetHandler(void* vFunc)
{
	gpio_intr_handler_register(vFunc, NULL);
	ETS_GPIO_INTR_ENABLE();
}


uint32 u32GPIOIntrStatus()
{
	// "rearm" the interrupt
	 uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	 GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status );
	 return gpio_status;
}
