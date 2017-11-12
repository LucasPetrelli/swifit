/**
 * @file spiffs_types.h
 * @author Lucas
 * @date Nov 5, 2017
 *
 * @brief
 */

#ifndef CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_TYPES_H_
#define CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_TYPES_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/**
 * @brief Filesystem handle
 */
typedef spiffs* thSPIFFS;

/**
 * @brief File handle type, originally an uint32
 */
typedef spiffs_file thSpiffsFileHandle;

/**
 * @brief struct with information about a file
 */
typedef struct {
	thSPIFFS hFS;					/**< The filesystem to which the file belongs to */
	thSpiffsFileHandle hFile;		/**< The file handle */
	char acName[8];					/**< The name of the file */
} tsSpiffsFile;

typedef struct{
	uint32 u32TotalSize;
	uint32 u32UsedSize;
	int32 i32Err;
} tsSpiffsStatus;

typedef struct{
	xSemaphoreHandle xMutex;
} tsSpiffsControl;


#endif /* CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_TYPES_H_ */
