/**
 * @file spiffs_adapter.h
 * @author Lucas
 * @date Oct 23, 2017
 *
 * @brief
 */

#ifndef CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_ADAPTER_H_
#define CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_ADAPTER_H_

#include "spiffs_impl.h"
#include "spiffs.h"
#include "spiffs_nucleus.h"
#include "spiffs_types.h"

#include "debug_adapter.h"

#define SPIFFS_FILE_NOK 0
/**
 * \defgroup adapter ESP8266 Adapter APIs
 * @brief ESP8266 SDK adaptation for the project
 */

/**
 * @addtogroup adapter
 * @{
 */

/**
 * \defgroup spiffs_adapter SPIFFS
 * @brief SPIFFS file system management API
 */

/**
 * @addtogroup spiffs_adapter
 * @{
 */


/**
 * @brief initializes the SPIFFS file system
 */
void vSPIFFSInit();

/**
 * @brief opens a file in SPIFFS
 *
 * @param  char* pcFilename: name of the file to be opened
 *
 * @return handle of the file
 */
tsSpiffsFile sSPIFFSOpen(thSPIFFS hFilesystem, const char* zFilename);

/**
 * @brief read data from a spiffs file
 */
int32 vSPIFFSRead(tsSpiffsFile sFile, char *pcData, uint32 u32Size);

/**
 * @brief write data to a spiffs file
 */
int32 vSPIFFSWrite(tsSpiffsFile sFile, char *pcData, uint32 u32Size);

/**
 * @brief closes an opened file in the filesystem
 */
void vSPIFFSClose(tsSpiffsFile sFile);

/**
 * @brief gets the status of a filesystem
 */
tsSpiffsStatus sSPIFFSStatus(thSPIFFS hFS);

/**
 * @brief gets the filesystem handle
 */
thSPIFFS hGetFilesystem(void);

/**
 * @brief take the spiffs mutex
 */
void vSPIFFSTakeMutex(void);

/**
 * @brief give back the spiffs semaphore
 */
void vSPIFFSGiveMutex(void);


void vSPIFFSTest();


/**
 * @}
 */

/**
 * @}
 */
#endif /* CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_ADAPTER_H_ */
