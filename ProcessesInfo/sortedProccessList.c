#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include <time.h>

#include "dllStruct.h"
#include "proccesStruct.h"
#include "sampleStruct.h"
#include "ProcessesInfo.h"
#include "SnapShot.h"
#include "GenerateHtml.h"
#include "sortedProccessList.h"
#include "log.h"
#pragma warning(disable:4996)

proccessList* Head;
proccessList* Tail;

void replace(proccessList* List) //replace items 
{
 
	if (List == NULL || List->next == NULL) //if list is empty or has only one item there's no need to sort
	{

	}
	else if (List == Head && List->next == Tail) //if list has 2 items
	{
		List->next = NULL;
		List->prev = Tail;
		Tail->prev = NULL;
		Tail->next = List;
		Tail = List;
		Head = List->prev;
	}//more than 2 items
	else if (List == Head && List->next != NULL) //if item to replace is the Head of list and next item is regular item(not Tail)
	{
		List->next = List->next->next;
		List->next->prev->next = List;
		List->next->prev->prev = NULL;
		List->prev = List->next->prev;
		List->next->prev = List;
		Head = List->prev;
	}
	else if (List->next == Tail) //if item to replace is not Head but next one is Tail
	{
		List->prev->next = List->next;
		List->next->prev = List->prev;
		List->next->next = List;
		List->prev = List->next;
		List->next = NULL;
		Tail = List;
	}
	else {
		// regular swap
		List->prev->next = List->next;
		List->next->prev = List->prev;
		List->next->next->prev = List;
		List->prev = List->next;
		List->next = List->next->next;
		List->prev->next = List;
	}

	Log_Event("Proccess item replaced (sorting proccess list by dll cnt).\n");
}

proccessList* sortByDll(proccessList* pList) //sort proccess list by dll cnt
{
	Head = pList;
	proccessList* curr = Head;

	while (curr != NULL)//get Tail to end of list
	{
		Tail = curr;
		curr = curr->next;
	}
	curr = Head;

	if (!curr) { //if list is empty there's nothing to sort
		return;
	}
	char change = 1;

	while (change != 0) //the loop will stop when the list is completly sort, there will be spare round
	{
		change = 0;
		while (curr != NULL)
		{
			//if this item is not last and this item's dll cnt is less the next one 
			if (curr->next != NULL && curr->dllCnt < curr->next->dllCnt) 
			{
				replace(curr); //replace items
				change++; //
			}
			curr = curr->next;
		}
		curr = Head; //initilize pointer to head of list
	}
	return curr;
}

proccessList* makeCopySortedList(proccessList* pList) //copy proccess list and sorting it by dll cnt
{
	proccessList* curr;
	proccessList* copy = NULL;

	while (pList != NULL)
	{
		curr = (proccessList*)malloc(sizeof(proccessList)); //memory allocation
		if (!curr) //allocation check
		{
			Log_Error(strerror(GetLastError()));
			printf("Error of memory\n");
			return;
		}
		Log_Event("Sorting proccess list by dll cnt.\n");

		curr->next = NULL;
		curr->prev = NULL;

		//copy values
		strcpy(curr->processName, pList->processName);
		curr->proccessId = pList->proccessId;
		curr->PageFaultCount = pList->PageFaultCount;
		curr->WorkingSetSize = pList->WorkingSetSize;
		curr->QuotaPagedPoolUsage = pList->QuotaPagedPoolUsage;
		curr->QuotaPeakPagedPoolUsage = pList->QuotaPeakPagedPoolUsage;
		curr->PagefileUsage = pList->PagefileUsage;
		curr->dList = pList->dList;
		curr->dllCnt = pList->dllCnt;

		if (copy == NULL) //if copied list is empty
		{
			copy = curr;
		}
		else { //if not empty
			while (copy->next != NULL) //get the pointer to tail of the copied list
			{
				copy = copy->next;
			}

			copy->next = curr;
			curr->prev = copy;
			copy = curr;
		}

		pList = pList->next;
	}
	if (copy) //if copied list created
	{
		while (copy->prev != NULL) //get the pointer to head of the copied list
		{
			copy = copy->prev;
		}
	}
	Log_Event("Sorted proccess list by dll cnt.\n");

	copy=sortByDll(copy); //sort the copied list by dll cnt

	return copy;
}