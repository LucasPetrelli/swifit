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


tsConfiguration prv_sConfig;
char prv_acName_[NAME_LENGTH];
tsTimingEntry* prv_psTable = NULL;
tsNetworkRule* prv_psRuleTable = NULL;


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

	prv_sConfig.eMode = (teOperationMode) pcFileContent[0];
	strncpy(prv_sConfig.acAP_, pcStartOfAP, u8LenOfAP);
	strncpy(prv_sConfig.acPassword_, pcStartOfPassword, u8LenOfPassword);
	strncpy(prv_sConfig.acMessage_, pcStartOfMessage, u8LenOfMessage);

vReadConfiguration_end:
	LOG_DEBUG("Info retrieved");
	LOG_DEBUG("Mode: %u", prv_sConfig.eMode);
	LOG_DEBUG("AP [%u]: %s", u8LenOfAP, prv_sConfig.acAP_);
	LOG_DEBUG("Password [%u]: %s", u8LenOfPassword, prv_sConfig.acPassword_);
	LOG_DEBUG("Message [%u]: %s", u8LenOfMessage, prv_sConfig.acMessage_);
	free(pcFileContent);

	if (zConfigurationGetName()==NULL)
	{
		if (TARGET_PRODUCT == PLUG_PROD)
		{
			vConfigurationSetName("Swifit Socket");
		}
		else
		{
			vConfigurationSetName("Swifit Switch");
		}
	}

	if (psConfigurationGetTimeTable() == NULL)
	{
		tsTimingEntry* psTable = (tsTimingEntry*)zalloc(sizeof(tsTimingEntry)*N_TIME_ENTRIES);
		vConfigurationSetTimeTable(psTable);
		free(psTable);
	}
	LOG_DEBUG("Device Name: [ %s ]", prv_acName_);

	if (psConfigurationGetRuleTable() == NULL)
	{
		tsNetworkRule* psTable = (tsNetworkRule*)zalloc(sizeof(tsNetworkRule)*N_RULE_ENTRIES);
		vConfigurationSetRuleTable(psTable);
		free(psTable);
	}
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
	sprintf(zFileContent, zConfigurationTemplate, psConfig->eMode, psConfig->acAP_, psConfig->acPassword_, psConfig->acMessage_);
	eWriteToFile((char*) zConfigurationFile, zFileContent, strlen(zFileContent));
	free(zFileContent);
	prv_sConfig = *psConfig;
}

tsConfiguration* psConfigurationGet()
{
	return &prv_sConfig;
}

void vConfigurationClearMessage()
{
	memset(prv_sConfig.acMessage_, '\0', strlen(prv_sConfig.acMessage_));
	vConfigurationSet(&prv_sConfig);
}

char* zConfigurationGetName()
{
	teException eException;
	if (strlen(prv_acName_) == 0)
	{
		eException = eReadFromFile("name", prv_acName_, NAME_LENGTH);
		if (eException != EX_SUCCESSFUL)
		{
			return NULL;
		}
	}
	return prv_acName_;
}

void vConfigurationSetName(char* zName)
{
	memset(prv_acName_, 0, NAME_LENGTH);
	strncpy(prv_acName_, zName, strlen(zName));
	eWriteToFile("name", prv_acName_, NAME_LENGTH);
}

uint8_t u8ConfigurationGetActuatorState()
{
	uint8_t u8State = 0;
	char zActuator[2];
	LOG_DEBUG("Read actuator state");
	teException eException = eReadFromFile("act", zActuator, 2);
	if (eException != EX_SUCCESSFUL)
	{
		LOG_DEBUG("Read fail");
		u8State = 0;
	}
	u8State = (uint8_t)strtoul(zActuator, NULL, 10);
	LOG_DEBUG("State read = [%u]",u8State);
	return u8State;
}

void vConfigurationSetActuatorState(uint8_t u8State)
{
	char zActuator[4];
	sprintf(zActuator, "%u", u8State%2);
	LOG_DEBUG("Write actuator state (%s)(%u)", zActuator, strlen(zActuator));
	eWriteToFile("act", zActuator, strlen(zActuator));
}

tsTimingEntry* psConfigurationGetTimeTable()
{
	if (prv_psTable != NULL)
	{
		return prv_psTable;
	}

	prv_psTable = (tsTimingEntry*)zalloc(sizeof(tsTimingEntry)*N_TIME_ENTRIES);
	teException eException = eReadFromFile("timetable", (char*) prv_psTable, sizeof(tsTimingEntry)*N_TIME_ENTRIES);
	if (eException != EX_SUCCESSFUL)
	{
		free(prv_psTable);
		prv_psTable = NULL;
		LOG_DEBUG("Failed to read table from memory!");
	}
	else
	{
		LOG_DEBUG("Retrieved table from memory");
	}
	return prv_psTable;
}

void vConfigurationSetTimeTable(tsTimingEntry* psTable)
{
	if (prv_psTable == NULL)
	{
		prv_psTable = (tsTimingEntry*)zalloc(sizeof(tsTimingEntry)*N_TIME_ENTRIES);
	}
	LOG_DEBUG("Setting table to:");
	vTimingPrintTable(psTable);
	memcpy((void*) prv_psTable, (void*) psTable, sizeof(tsTimingEntry)*N_TIME_ENTRIES);
	eWriteToFile("timetable", (char*) psTable, sizeof(tsTimingEntry)*N_TIME_ENTRIES);
}


tsNetworkRule* psConfigurationGetRuleTable()
{
	if (prv_psRuleTable != NULL)
	{
		return prv_psRuleTable;
	}

	prv_psRuleTable = (tsNetworkRule*)zalloc(sizeof(tsNetworkRule)*N_RULE_ENTRIES);
	teException eException = eReadFromFile("ruletable", (char*) prv_psRuleTable, sizeof(tsTimingEntry)*N_RULE_ENTRIES);
	if (eException != EX_SUCCESSFUL)
	{
		free(prv_psRuleTable);
		prv_psRuleTable = NULL;
		LOG_DEBUG("Failed to read table from memory!");
	}
	else
	{
		LOG_DEBUG("Retrieved table from memory");
	}
	return prv_psRuleTable;
}

void vConfigurationSetRuleTable(tsNetworkRule* psTable)
{
	if (prv_psRuleTable == NULL)
	{
		prv_psRuleTable = (tsNetworkRule*)zalloc(sizeof(tsNetworkRule)*N_RULE_ENTRIES);
	}
	LOG_DEBUG("Setting table to:");
	vDeviceLogRuleTable(psTable);
	memcpy((void*) prv_psRuleTable, (void*) psTable, sizeof(tsNetworkRule)*N_RULE_ENTRIES);
	eWriteToFile("ruletable", (char*) psTable, sizeof(tsNetworkRule)*N_RULE_ENTRIES);
}
