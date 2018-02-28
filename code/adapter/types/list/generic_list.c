/*
 * list.c
 *
 *  Created on: Feb 25, 2018
 *      Author: Lucas
 */

#include "basictypes.h"
#include "generic_list.h"


tpList vListCreate()
{
	tpList pList = (tsListMember**)zalloc(sizeof(tsListMember*));
	return pList;
}

void vListDestroy(tpList pList)
{
	free(pList);
}

void vListAppend(tpList pList, tsListMember* psMember)
{
	if (pList == NULL)
	{
		return;
	}

	tsListMember* psCurrentMember = *pList;
	if (psCurrentMember)
	{
		while (psCurrentMember->pvNext)
		{
			psCurrentMember = psCurrentMember->pvNext;
		}
		psCurrentMember->pvNext = (void*) psMember;
		psMember->pvPrev = (void*) psCurrentMember;
	}
	else
	{
		*pList = psMember;
		psMember->pvNext = NULL;
		psMember->pvPrev = NULL;
	}
}

void vListRemove(tpList pList, tsListMember* psMember)
{
	if (pList == NULL)
	{
		return;
	}

	tsListMember* psCurrentMember = *pList;
	while (psCurrentMember != NULL)
	{
		if (psCurrentMember == psMember)
		{
			if (psCurrentMember == *pList)
			{
				*pList = psCurrentMember->pvNext;
				if (psCurrentMember->pvNext)
				{
					((tsListMember*)psCurrentMember->pvNext)->pvPrev = NULL;
				}
			}
			else
			{
				((tsListMember*)psCurrentMember->pvPrev)->pvNext = psCurrentMember->pvNext;
				if (psCurrentMember->pvNext)
				{
					((tsListMember*)psCurrentMember->pvNext)->pvPrev = psCurrentMember->pvPrev;
				}
			}
		}
	}
}
