/*
 * devices.c
 *
 *  Created on: Feb 26, 2018
 *      Author: Lucas
 */
#include "devices.h"
#include "wifi_adapter.h"
#include "system_adapter.h"
#include "product_config.h"
void vDeviceAppend(xListOfDevices* pxDevList, tsDevice* psDev)
{
	psDev->pvNext = NULL;
	if (*pxDevList == NULL)
	{
		*pxDevList = psDev;
		return;
	}

	tsDevice* psCurrent = (tsDevice*)*pxDevList;
	while(psCurrent->pvNext != NULL)
	{
		psCurrent = (tsDevice*) psCurrent->pvNext;
	}
	psCurrent->pvNext = psDev;
}

void vDeviceRemove(xListOfDevices* pxDevList, tsDevice* psDev)
{
	if (pxDevList == NULL || psDev == NULL)
	{
		return;
	}
	if (*pxDevList == NULL)
	{
		return;
	}

	tsDevice* psCurrent = (tsDevice*)*pxDevList;
	if (psCurrent == psDev)
	{
		// Device is the first of the list
		*pxDevList = (tsDevice*)psCurrent->pvNext;
		return;
	}
	tsDevice* psPrev;
	while(psCurrent)
	{
		psPrev = psCurrent;
		psCurrent = (tsDevice*) psCurrent->pvNext;
		if (psCurrent == psDev)
		{
			psPrev->pvNext = psCurrent->pvNext;
			return;
		}
	}


}

void vDeviceUpdate(tsDevice* psReceiver, tsDevice* psModel)
{
	memcpy((void*)(((uint8_t*)psReceiver) + 4), (void*)(((uint8_t*)psModel) + 4), sizeof(tsDevice)-4);
}

tsDevice* psDeviceGetById(xListOfDevices* pxDevList, uint32_t u32Id)
{
	tsDevice* psCurrent = *pxDevList;
	while(psCurrent)
	{
		if (psCurrent->u32ID == u32Id)
		{
			return psCurrent;
		}
		psCurrent = (tsDevice*)psCurrent->pvNext;
	}
	return (tsDevice*) NULL;
}

tsDevice* psDeviceGetSelf()
{
	tsDevice* psDev = (tsDevice*)zalloc(sizeof(tsDevice));

	eWifiRetrieveCurrentIP(psDev->u8IP_);
	psDev->u32ID = u32SystemGetId();

    if (TARGET_PRODUCT == PLUG_PROD)
    {
    	psDev->eType = DEVICE_SOCKET;
        psDev->eSensorState = eSensorGetState(PIR);
    }
    else
    {
    	psDev->eType = DEVICE_SWITCH;
        psDev->eSensorState = eSensorGetState(SWITCH);
    }
    psDev->eActuatorState = eActuatorGetStateById(RELAY);
	return psDev;
}


void vDeviceLog(tsDevice* psDev)
{
	LOG_DEBUG("---- DEVICE ----");
	LOG_DEBUG("ID:\t%u", psDev->u32ID);
	LOG_DEBUG("IP:\t%u.%u.%u.%u", psDev->u8IP_[0], psDev->u8IP_[1], psDev->u8IP_[2], psDev->u8IP_[3]);
	LOG_DEBUG("Name:\t%s", psDev->cName_);
	LOG_DEBUG("Count:\t%u", psDev->u8Countdown);
	LOG_DEBUG("Type:\t%u", psDev->eType);
	LOG_DEBUG("Actuator state:\t%u", psDev->eActuatorState);
	LOG_DEBUG("Sensor state:\t%u", psDev->eSensorState);
	LOG_DEBUG("---- ------ ----");
}

void vDeviceLogList(xListOfDevices psDevList)
{
	tsDevice* psDev = (tsDevice*) psDevList;
	LOG_DEBUG("---- DEVICE LIST ----");
	while(psDev)
	{
		LOG_DEBUG(
				"ID: %u\t\tIP: %u.%u.%u.%u",
				psDev->u32ID,
				psDev->u8IP_[0],
				psDev->u8IP_[1],
				psDev->u8IP_[2],
				psDev->u8IP_[3]
		);
		psDev = (tsDevice*) psDev->pvNext;
	}
	LOG_DEBUG("---- ----------- ----");
}
