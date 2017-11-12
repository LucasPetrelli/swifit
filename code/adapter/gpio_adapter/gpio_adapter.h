/**
 * @file gpio_adapter.h
 * @author Lucas
 * @date Oct 9, 2017
 *
 * @brief ESP8266 GPIO adapter implementation header file
 *
 *
 */

#ifndef CODE_ADAPTER_GPIO_ADAPTER_GPIO_ADAPTER_H_
#define CODE_ADAPTER_GPIO_ADAPTER_GPIO_ADAPTER_H_

// Driver includes
#include "gpio.h"

// Adapter includes
#include "basictypes.h"

/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 */

/**
 * @addtogroup adapter
 * @{
 */

/**
 * \defgroup gpio_adapter GPIO
 * @brief GPIO handling API
 *
 */

/**
 * @addtogroup gpio_adapter
 * @{
 */

/**
 * @Brief GPIO mode as input/output enumerator
 */
typedef enum {
	GPIO_Output,		/**< GPIO as output */
	GPIO_Input			/**< GPIO as input */
} teGPIOMode;

/**
 * @Brief GPIO number enumerator
 */
typedef enum {
	GPIO_B0=(1<<0),
	GPIO_B1=(1<<1),
	GPIO_B2=(1<<2),
	GPIO_B3=(1<<3),
	GPIO_B4=(1<<4),
	GPIO_B5=(1<<5),
	GPIO_B6=(1<<6),
	GPIO_B7=(1<<7),
	GPIO_B8=(1<<8),
	GPIO_B9=(1<<9),
	GPIO_B10=(1<<10),
	GPIO_B11=(1<<11),
	GPIO_B12=(1<<12),
	GPIO_B13=(1<<13),
	GPIO_B14=(1<<14),
	GPIO_B15=(1<<15),
} teGPIOBit;

/**
 * @Brief GPIO Interrupt trigger mode
 */
typedef enum {
	GPIO_INTR_DISABLE = 0,
	GPIO_INTR_POSEDGE = 1,
	GPIO_INTR_NEGEDGE = 2,
	GPIO_INTR_ANYEDGE = 3,
	GPIO_INTR_LOW = 4,
	GPIO_INTR_HIGH = 5
} teGPIOIntrMode;

typedef enum {
	GPIO_0,
	GPIO_1,
	GPIO_2,
	GPIO_3,
	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
} teGPIONumber;


/**
 * @Brief GPIO input value enumerator
 */
typedef enum {
	LOW,
	HIGH,
} teGPIOValue;

/**
 * @brief GPIO mode configuration API
 *
 *  Use this function to configure a set of GPIO as input or output
 *
 *  @param u16GPIOBitfield : Bitfield depicting the GPIOs that will be configured (bit set)
 *  @param eMode : either GPIO_Output or GPIO_Input
 */
void vGPIOSetIOMode(uint16 u16GPIOBitfield, teGPIOMode eMode);

/**
 * @brief GPIO read API
 *
 *  Use this function to read the value on a GPIO configured as input
 *
 *  @param eNumber : the number of the GPIO to be read
 *  @return either (teGPIOValue) LOW or HIGH
 */
teGPIOValue eGPIORead(teGPIONumber eNumber);

/**
 * @brief GPIO write API
 *
 *  Use this function to write a value to a GPIO configured as output
 *
 *  @param eGPIOGroup : the group of GPIO bits to be written
 *  @param eValue : the value to be written to the GPIO
 */
void vGPIOWrite(teGPIOBit eGPIOGroup, teGPIOValue eValue);

/**
 * @brief GPIO interrupt configuration function
 *
 * Use this function to assign a pin to trigger the GPIO interrupt
 *
 * @param eNumber : the gpio number to be set as a trigger
 * @param eMode : the mode of the GPIO trigger
 */
void vGPIOIntrSetTrigger(teGPIONumber eNumber, teGPIOIntrMode eMode);

/**
 * @brief GPIO interrupt handler assignment
 *
 * Use this function to assign a function to handle the GPIO interrupt
 *
 * @param vFunc : pointer to the function that handles the ISR. Should have signature like
 * 				  void isrHandler(uint32t mask, void* args);
 */
void vGPIOIntrSetHandler(void* vFunc);

/**
 * @brief Clears and rearm the GPIO interrupt
 *
 */
uint32 u32GPIOIntrStatus();

/**
 * @}
 */

/**
 * @}
 */

#endif /* CODE_ADAPTER_GPIO_ADAPTER_GPIO_ADAPTER_H_ */
