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
 * @brief Writes data to a file
 *
 * @param zFilename : zero-terminated string naming the file
 * @param pcData	: pointer to the character typed data
 * @param u32Len	: length of the data to write
 */
teException eReadFromFile(char* zFilename, char* pcData, uint32 u32Len);

/**
 * @brief Reads data from a file
 */
teException eWriteToFile(char* zFilename, char* pcData, uint32 u32Len);



#endif /* CODE_MODULE_FILESYSTEM_FILESYSTEM_H_ */
