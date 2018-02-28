/*
 * list.h
 *
 *  Created on: Feb 25, 2018
 *      Author: Lucas
 */

#ifndef CODE_ADAPTER_TYPES_LIST_GENERIC_LIST_H_
#define CODE_ADAPTER_TYPES_LIST_GENERIC_LIST_H_

struct tsListMember;

typedef struct {
	void* pvNext;
	void* pvPrev;
} tsListMember;


typedef tsListMember** tpList;

void vListAppend(tpList pList, tsListMember* psMember);
void vListRemove(tpList pList, tsListMember* psMember);

#endif /* CODE_ADAPTER_TYPES_LIST_GENERIC_LIST_H_ */
