/**
 * @file system_adapter.h
 * @author Lucas
 * @date Nov 9, 2017
 *
 * @brief
 */

#ifndef CODE_ADAPTER_SYSTEM_ADAPTER_SYSTEM_ADAPTER_H_
#define CODE_ADAPTER_SYSTEM_ADAPTER_SYSTEM_ADAPTER_H_

#include "debug_adapter.h"
#include "esp_common.h"
#include "espconn.h"
#include "uart.h"

/**
 * @brief System specific setup encapsulation
 */
void vSystemInit();

void vTaskSystemHeapReport(void* pvParameters);


#endif /* CODE_ADAPTER_SYSTEM_ADAPTER_SYSTEM_ADAPTER_H_ */
