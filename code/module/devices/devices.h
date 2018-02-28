/*
 * devices.h
 *
 *  Created on: Feb 26, 2018
 *      Author: Lucas
 */

#ifndef CODE_MODULE_DEVICES_DEVICES_H_
#define CODE_MODULE_DEVICES_DEVICES_H_

#include "devices_types.h"

#define DEVDEBUG

#ifdef DEVDEBUG
	#define LOG_DEV(dev) vDeviceLog(dev)
	#define LOG_DEV_LIST(dev) vDeviceLogList(dev)
#else
	#define LOG_DEV(dev) {do {} while(0); }
	#define LOG_DEV_LIST(dev) {do {} while(0); }
#endif

void vDeviceAppend(xListOfDevices* pxDevList, tsDevice* psDev);

void vDeviceRemove(xListOfDevices* pxDevList, tsDevice* psDev);

void vDeviceUpdate(tsDevice* psReceiver, tsDevice* psModel);

tsDevice* psDeviceGetById(xListOfDevices* pxDevList, uint32_t u32Id);

tsDevice* psDeviceGetSelf();

void vDeviceLog(tsDevice* psDev);

void vDeviceLogList(xListOfDevices psDevList);


#endif /* CODE_MODULE_DEVICES_DEVICES_H_ */
