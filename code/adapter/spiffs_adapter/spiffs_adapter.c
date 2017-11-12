/**
 * @file spiffs_adapter.c
 * @author Lucas
 * @date Oct 23, 2017
 *
 * @brief
 */
#include "spiffs_adapter.h"

tsSpiffsControl sSpiffsControl;

void vSPIFFSInit()
{
	spiffs_fs1_init();
	fs_mount_specific(FS1_FLASH_ADDR, FS1_FLASH_SIZE, SECTOR_SIZE, LOG_BLOCK, LOG_PAGE);

	sSpiffsControl.xMutex = xSemaphoreCreateMutex();

	if (sSpiffsControl.xMutex == NULL)
	{
		LOG_DEBUG("Could not create mutex!");
		vTaskDelete(NULL);
	}
}

tsSpiffsFile sSPIFFSOpen(thSPIFFS hFilesystem, const char* zFilename)
{
	tsSpiffsFile sFile;
	memcpy(sFile.acName, zFilename, strlen(zFilename));
	sFile.hFS = hFilesystem;
	sFile.hFile = SPIFFS_open(hFilesystem, (char*) zFilename, SPIFFS_CREAT|SPIFFS_RDWR, 0);
	if (sFile.hFile < 0)
	{
		sFile.hFile = sSPIFFSStatus((spiffs*) sFile.hFS).i32Err;
		printf("Error %i while opening %s", sFile.hFile, zFilename);
	}
	return sFile;
}

int32 vSPIFFSRead(tsSpiffsFile sFile, char *pcData, uint32 u32Size)
{
	int32 i32Ret = SPIFFS_read((spiffs*) sFile.hFS, (spiffs_file) sFile.hFile, pcData, u32Size);
	if (i32Ret < 0) {
		return sSPIFFSStatus((spiffs*) sFile.hFS).i32Err;
	}
	else
	{
		return i32Ret;
	}
}

int32 vSPIFFSWrite(tsSpiffsFile sFile, char *pcData, uint32 u32Size)
{
	int32 i32Ret = (int32) SPIFFS_write((spiffs*) sFile.hFS, (spiffs_file) sFile.hFile, pcData, u32Size);
	if (i32Ret < 0) {
		return sSPIFFSStatus((spiffs*) sFile.hFS).i32Err;
	}
	else
	{
		return i32Ret;
	}
}

void vSPIFFSClose(tsSpiffsFile sFile)
{
	SPIFFS_close((spiffs*) sFile.hFS, (spiffs_file) sFile.hFile);
}

thSPIFFS hGetFilesystem(void)
{
	return (thSPIFFS) psGetFilesystemHandle();
}


tsSpiffsStatus sSPIFFSStatus(thSPIFFS hFS)
{
	tsSpiffsStatus sStatus;
	SPIFFS_info((spiffs*) hFS, &sStatus.u32TotalSize, &sStatus.u32UsedSize);
	sStatus.i32Err = SPIFFS_errno((spiffs*) hFS);
	return sStatus;
}

void vSPIFFSTest()
{
	const char zFilename[] = "file1";
	char zData[] = "some data\n";
	char *pcReadData;
	int32 i32Feedback;

	tsSpiffsFile sFile = sSPIFFSOpen(hGetFilesystem(), zFilename);
	printf("Filename: %s\n"
			"Filehandle: %i\n"
			"Errno: %i\n", sFile.acName, sFile.hFile, SPIFFS_errno(hGetFilesystem()));
	i32Feedback = vSPIFFSWrite(sFile, zData, strlen(zData)+1);
	vSPIFFSClose(sFile);

	printf("Wrote %i data\n", i32Feedback);

	sFile = sSPIFFSOpen(hGetFilesystem(), zFilename);
	pcReadData = (char*) zalloc(strlen(zData));
	i32Feedback = vSPIFFSRead(sFile, pcReadData, strlen(zData)+1);
	vSPIFFSClose(sFile);

	printf("Read %i data\n", i32Feedback);
	printf("Write and read something: %s\n", pcReadData);
	free(pcReadData);

	uint32 u32Total;
	uint32 u32Used;
	SPIFFS_info(hGetFilesystem(), &u32Total, &u32Used);
	printf("FS Size: %i --- Used %i", u32Total, u32Used);

}

void vSPIFFSTakeMutex(void)
{
	xSemaphoreTake(sSpiffsControl.xMutex, portMAX_DELAY);
}

void vSPIFFSGiveMutex(void)
{
	xSemaphoreGive(sSpiffsControl.xMutex);
}
