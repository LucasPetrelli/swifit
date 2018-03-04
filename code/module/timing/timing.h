/*
 * timing.h
 *
 *  Created on: Mar 2, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_TIMING_TIMING_H_
#define CODE_MODULE_TIMING_TIMING_H_

/**
 * \defgroup module Functional Modules
 * @brief Functional modules for handling a specific task
 * @addtogroup module
 * @{
 * \defgroup timing Timing
 * @brief Handling of timing elements
 * @addtogroup timing
 * @{
 * \defgroup timing_api API
 * @brief Timing function definitions
 * @addtogroup timing_api
 * @{
 */

#include "sntp_adapter.h"
#include "timing_types.h"

/**
 * @brief Retrieves a struct with decoded fields from a timestamp
 * @param u32Timestamp the unix timestamp to be broken down into information
 * @return A struct with date and time fields
 */
tsTime* psTimingTimestampToStruct(uint32_t u32Timestamp);

/**
 * @brief Retrieves the count of minutes since the beginning of the day psTime represents
 * @param psTime a struct with decoded date and time fields
 * @return The amount of minutes since 00:00 from the day represented by psTime
 */
uint32_t u32TimingMinutes(tsTime* psTime);

/**
 * @brief Retrieves the count of minutes since the beginning of the day u32Timestamp represents
 * @param u32Timestamp the unix timestamps to be parsed
 * @return The amount of minutes since 00:00 from the day represented by u32Timestamp
 */
uint32_t u32TimingMinutesFromTimestamp(uint32_t u32Timestamp);

void vTimingPrintTable(tsTimingEntry* psTable);
#endif /* CODE_MODULE_TIMING_TIMING_H_ */
