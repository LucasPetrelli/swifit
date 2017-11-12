/**
 * @file testgpiotask.c
 * @author Lucas
 * @date Oct 9, 2017
 *
 * @brief
 */

#include "freertos/FreeRTOS.h"
#include "freertos/Task.h"

#include "gpio_adapter.h"

void vHandler(uint32 mask, void* args)
{
	teGPIOValue eLedStatus = eGPIORead(GPIO_4);
	if (eLedStatus == LOW)
	{
		eLedStatus = HIGH;
	}
	else
	{
		eLedStatus = LOW;
	}
	vGPIOWrite(GPIO_B2, eLedStatus);
	u32GPIOIntrStatus();
}

void vTaskTestGpio(void* args)
{
	teGPIOBit eLedGpioBit = GPIO_B2;
	teGPIOBit ePIRGpioBit = GPIO_B4;
	teGPIOValue eLedStatus = LOW;
	vGPIOSetIOMode(eLedGpioBit, GPIO_Output);
	vGPIOSetIOMode(ePIRGpioBit, GPIO_Input);

	vGPIOIntrSetHandler(vHandler);
	vGPIOIntrSetTrigger(GPIO_4, GPIO_INTR_POSEDGE);
	while (TRUE)
	{
		//vGPIOWrite(eLedGpioBit, eGPIORead(GPIO_4));
		vTaskDelay( 1000/portTICK_RATE_MS );
	}
}


