#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "dllStruct.h"
#include "log.h"
#pragma warning(disable:4996)

dllList* dllHead = NULL;
dllList* dllTail = NULL;

dllList* addDllItem(char dllName[500]) //add dll item to the list
{
	dllList* curr;
	curr = (dllList*)malloc(sizeof(dllList)); //memory allocation
	if (!curr) //allocation check
	{
		Log_Error(strerror(GetLastError()));
		return;
	}
	//Log_Event("adding dll item.\n");

	curr->next = NULL;
	curr->prev = NULL;

	strcpy(curr->dllName, dllName); //put values in item

	if (dllHead == NULL) //if list is empty
	{
		dllHead = curr;
		dllTail = curr;
	}
	else //if not empty 
	{
		dllTail->next = curr;
		curr->prev = dllTail;
		dllTail = curr;
	}

	return dllHead;
}

int searchDllByName(char Name[500], dllList* search) //search if dll item is in the search list by its name
{
	dllList* curr = search;

	if (curr == NULL) //list is empty- not found- return 1
	{
		return 1;
	}

	while (curr != NULL)
	{
		if (strcmp(curr->dllName, Name)==0) //if found- return 0
		{
			return 0;
		}
		curr = curr->next;
	}
	return 1; //not found- return 1
}

void resetDllList() //reset dllHead/Tail to NULL
{
	dllHead = dllTail = NULL;
	Log_Event("Reset Dll List.\n");
}

unsigned short getDllCntByDllList(dllList* search) //count dll in search list
{
	unsigned short dllCnt = 0;
	dllList* curr = search;
	Log_Event("Counting all sample's dll.\n");

	while (curr != NULL)
	{
		dllCnt = dllCnt++;
		curr = curr->next;
	}

	return dllCnt;
}

void freeDllMemory(dllList* dList) //free dll list memory
{
	dllList* currentDll = dList;
	dllList* releaseDll = dList;

	Log_Event("Free Dll memory.\n");

	while (currentDll != NULL)
	{
		releaseDll = currentDll;
		currentDll = currentDll->next;
		free(releaseDll);
	}
}
