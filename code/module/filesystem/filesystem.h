/**
 * @file filesystem.h
 * @author Lucas
 * @date Nov 5, 2017
 *
 * @brief Functions that manage data on the filesystem
 */

#ifndef CODE_MODULE_FILESYSTEM_FILESYSTEM_H_
#define CODE_MODULE_FILESYSTEM_FILESYSTEM_H_

#include "basictypes.h"
#include "spiffs_adapter.h"

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 *
 * \defgroup filesystem File System
 * @brief Handling of the device flash file system
 *
 * These functions use the SPIFFS adapter to make safe access to the flash memory,
 * lowering the system vulnerability to corruption.
 * @{
 */

/**
 * @brief Read data to a file
 *
 * @param zFilename : zero-terminated string naming the file
 * @param pcData	: pointer to the character typed data
 * @param u32Len	: length of the data to write
 * @return EX_SUCCESSFUL, EX_FILESYSTEM_OPEN_ERROR, EX_FILESYSTEM_RD_ERROR
 */
teException eReadFromFile(char* zFilename, char* pcData, uint32 u32Len);

/**
 * @brief Writes data from a file
 * @param zFilename : zero-terminated string naming the file
 * @param pcData	: pointer to the character typed data
 * @param u32Len	: length of the data to write
 * @return EX_SUCCESSFUL, EX_FILESYSTEM_OPEN_ERROR, EX_FILESYSTEM_WR_ERROR
 */
teException eWriteToFile(char* zFilename, char* pcData, uint32 u32Len);

/**
 * @}
 * @}
 */

#endif /* CODE_MODULE_FILESYSTEM_FILESYSTEM_H_ */
