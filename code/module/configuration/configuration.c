/*
 * configuration.c
 *
 *  Created on: Jan 28, 2018
 *      Author: Lucas
 */

#include "configuration.h"
#include "filesystem.h"

#define CONFIG_FILE_LENGTH 255
const int u8MaxSizeOfConfigFile = CONFIG_FILE_LENGTH;
const char* zConfigurationFile = "configuration";
const char* zConfigurationTemplate = (
		"%c\n"
		"%s\n"
		"%s\n"
		"%s\n"
);
const char zDefaultConfigurationContent[CONFIG_FILE_LENGTH] = "0\n"
		"\n"
		"\n"
		"\n";


tsConfiguration sConfig;

void vConfigurationRead()
{
	const uint32 u32SizeOfRead = u8MaxSizeOfConfigFile;
	teException eException = EX_GENERIC_FAIL;
	char* pcFileContent = (char*) zalloc(u32SizeOfRead);

#ifndef REPROGRAM
	eException = eReadFromFile((char*) zConfigurationFile, pcFileContent, u32SizeOfRead);
	if (eException != EX_SUCCESSFUL)
#endif
	{
		vConfigurationSetDefault();
		strcpy(pcFileContent, (char*) zDefaultConfigurationContent);

	}

	LOG_DEBUG("File content:\n%s", pcFileContent);
	char* pcStartOfMode = pcFileContent;
	char* pcStartOfAP = NULL;
	uint8 u8LenOfAP = 0;
	char* pcStartOfPassword = NULL;
	uint8 u8LenOfPassword = 0;
	char* pcStartOfMessage = NULL;
	uint8 u8LenOfMessage = 0;
	char* pcEndOfFile = NULL;

	pcStartOfAP = strchr(pcStartOfMode, '\n') + 1;
	pcStartOfPassword = strchr(pcStartOfAP, '\n') + 1;
	pcStartOfMessage = strchr(pcStartOfPassword, '\n') + 1;
	pcEndOfFile = strchr(pcStartOfMessage, '\n');

	u8LenOfAP = pcStartOfPassword - pcStartOfAP - 1;
	u8LenOfPassword = pcStartOfMessage - pcStartOfPassword - 1;
	u8LenOfMessage = pcEndOfFile - pcStartOfMessage;

	sConfig.eMode = (teOperationMode) pcFileContent[0];
	strncpy(sConfig.acAP, pcStartOfAP, u8LenOfAP);
	strncpy(sConfig.acPassword, pcStartOfPassword, u8LenOfPassword);
	strncpy(sConfig.acMessage, pcStartOfMessage, u8LenOfMessage);


vReadConfiguration_end:
	LOG_DEBUG("Info retrieved");
	LOG_DEBUG("Mode: %u", sConfig.eMode);
	LOG_DEBUG("AP [%u]: %s", u8LenOfAP, sConfig.acAP);
	LOG_DEBUG("Password [%u]: %s", u8LenOfPassword, sConfig.acPassword);
	LOG_DEBUG("Message [%u]: %s", u8LenOfMessage, sConfig.acMessage);
	free(pcFileContent);
	return;
}

void vConfigurationSetDefault()
{
	LOG_DEBUG("Creating default configuration file");
	eWriteToFile((char*) zConfigurationFile, (char*) zDefaultConfigurationContent, strlen(zDefaultConfigurationContent));
}

void vConfigurationSet(tsConfiguration* psConfig)
{
	char* zFileContent = (char*)zalloc(u8MaxSizeOfConfigFile);
	sprintf(zFileContent, zConfigurationTemplate, psConfig->eMode, psConfig->acAP, psConfig->acPassword, psConfig->acMessage);
	eWriteToFile((char*) zConfigurationFile, zFileContent, strlen(zFileContent));
	free(zFileContent);
	sConfig = *psConfig;
}

tsConfiguration* psConfigurationGet()
{
	return &sConfig;
}

void vConfigurationClearMessage()
{
	memset(sConfig.acMessage, '\0', strlen(sConfig.acMessage));
	vConfigurationSet(&sConfig);
}
