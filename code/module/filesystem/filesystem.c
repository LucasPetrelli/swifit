/**
 * @file filesystem.c
 * @author Lucas
 * @date Nov 5, 2017
 *
 * @brief
 */

#include "filesystem.h"

teException eReadFromFile(char* zFilename, char* pcData, uint32 u32Len)
{
	teException eException = EX_SUCCESSFUL;
	int32 i32ReadResult = 0;

	vSPIFFSTakeMutex();

	thSPIFFS hFS = hGetFilesystem();
	tsSpiffsFile sFile = sSPIFFSOpen(hFS, zFilename);
	if (sFile.hFile <= 0)
	{
		SET_EXCEPTION(EX_FILESYSTEM_OPEN_ERROR);
		goto eReadFromFile_end;
	}
	i32ReadResult = vSPIFFSRead(sFile, pcData, u32Len);
	if (i32ReadResult <= 0)
	{
		SET_EXCEPTION(EX_FILESYSTEM_RD_ERROR);
	}
	LOG_DEBUG("Read result: %u", i32ReadResult);
	vSPIFFSClose(sFile);

eReadFromFile_end:
	vSPIFFSGiveMutex();
	return eException;
}

teException eWriteToFile(char* zFilename, char* pcData, uint32 u32Len)
{
	teException eException = EX_SUCCESSFUL;
	int32 i32ReadResult = 0;

	vSPIFFSTakeMutex();

	thSPIFFS hFS = hGetFilesystem();
	tsSpiffsFile sFile = sSPIFFSOpen(hFS, zFilename);
	if (sFile.hFile <= 0)
	{
		SET_EXCEPTION(EX_FILESYSTEM_OPEN_ERROR);
		goto eWriteToFile_end;
	}
	i32ReadResult = vSPIFFSWrite(sFile, pcData, u32Len);
	if (i32ReadResult <= 0)
	{
		SET_EXCEPTION(EX_FILESYSTEM_WR_ERROR);
	}
	vSPIFFSClose(sFile);

eWriteToFile_end:
	vSPIFFSGiveMutex();
	return eException;
}


