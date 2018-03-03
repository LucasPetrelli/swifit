/**
 * sntp_adapter.c
 *
 *  Created on: Mar 2, 2018
 *      Author: Lucas
 */


#include "sntp_adapter.h"
#include "apps/sntp.h"
#include "apps/sntp_time.h"

void vSNTPInit()
{
	ip_addr_t sIp;
	IP4_ADDR(&sIp, 200,160,7,186);
	sntp_set_timezone(-3);
	sntp_init();
}

uint32_t u32SNTPGetTimestamp()
{
	return (uint32_t) sntp_get_current_timestamp();
}
