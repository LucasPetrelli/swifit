/*
 * timing.c
 *
 *  Created on: Mar 2, 2018
 *      Author: Lucas
 */

#include "timing.h"
#include "apps/sntp_time.h"
#include "debug_adapter.h"
#include "devices_types.h"

tsTime* psTimingTimestampToStruct(uint32_t u32Timestamp)
{
	tsTime* psTime = (tsTime*)zalloc(sizeof(tsTime));
	timet_to_tm((time_t) u32Timestamp, psTime);
	return psTime;
}

uint32_t u32TimingMinutes(tsTime* psTime)
{
	return ((uint32_t)psTime->Hour)*60 + psTime->Minute;
}

uint32_t u32TimingMinutesFromTimestamp(uint32_t u32Timestamp)
{
	tsTime* psTime = psTimingTimestampToStruct(u32Timestamp);
	uint32_t u32Minutes = u32TimingMinutes(psTime);
	free(psTime);
	return u32Minutes;
}


void vTimingPrintTable(tsTimingEntry* psTable)
{
	LOG_DEBUG("---- TABLE ----");
	LOG_DEBUG(
		"Status\t\tAction\t\tTime"
	);
	uint8_t u8Index = 0;
	for (;u8Index < N_TIME_ENTRIES; u8Index++)
	{
		LOG_DEBUG(
			"%u\t\t\t%u\t\t\t%u",
			psTable[u8Index].eEnabled,
			psTable[u8Index].eAction,
			psTable[u8Index].u16Minutes
		);
	}
}
