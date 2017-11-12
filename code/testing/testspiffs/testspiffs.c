/**
 * @file testspiffs.c
 * @author Lucas
 * @date Oct 23, 2017
 *
 * @brief
 */


#include "esp_common.h"


void vTaskTestSPIFFS(void* args)
{
	vSPIFFSInit();
	while(1)
	{
		vSPIFFSTest();
		printf("\n\n");
		vTaskDelay( 1000/portTICK_RATE_MS );
	}
}
