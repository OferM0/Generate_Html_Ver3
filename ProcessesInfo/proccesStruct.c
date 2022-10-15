#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "dllStruct.h"
#include "proccesStruct.h"
#include "log.h"
#pragma warning(disable:4996)

proccessList* proccessHead = NULL;
proccessList* proccessTail = NULL;

//add proccess item to the list
 proccessList* addProccessItem(char processName[1000], DWORD proccessId, DWORD  PageFaultCount, SIZE_T WorkingSetSize, SIZE_T QuotaPagedPoolUsage, SIZE_T QuotaPeakPagedPoolUsage, SIZE_T PagefileUsage, dllList* dList, unsigned short dllCnt)
 {
	 proccessList* cur;
	 cur=(proccessList*)malloc(sizeof(proccessList)); //memory allocation
	 if (!cur) //allocation check
	 {
		 Log_Error(strerror(GetLastError()));
		 return;
	 }
	 //Log_Event("adding proccess item.\n");

	 cur->next = NULL;
	 cur->prev = NULL;

	 strcpy(cur->processName, processName); //put values in item
	 cur->proccessId = proccessId;
	 cur->PageFaultCount = PageFaultCount;
	 cur->WorkingSetSize = WorkingSetSize;
	 cur->QuotaPagedPoolUsage = QuotaPagedPoolUsage;
	 cur->QuotaPeakPagedPoolUsage = QuotaPeakPagedPoolUsage;
	 cur->PagefileUsage = PagefileUsage;
	 cur->dList = dList;
	 cur->dllCnt = dllCnt;

	 if (proccessHead == NULL) //if list is empty
	 {
		 proccessHead = cur;
		 proccessTail = cur;
	 }
	 else //if not empty
	 {
		 proccessTail->next = cur;
		 cur->prev = proccessTail;
		 proccessTail = cur;
	 }

	 return proccessHead;
 }
 
 int searchProccessByName(char Name[1000], proccessList* search) //search if proccess item is in the search list by its name only
 {
	 proccessList* curr = search;

	 if (curr == NULL) //list is empty- not found- return 1
	 {
		 return 1;
	 }

	 while (curr != NULL)
	 {
		 if (strcmp(curr->processName, Name) == 0) //if found- return 0
		 {
			 return 0;
		 }
		 curr = curr->next;
	 }
	 return 1; //not found- return 1
 }

 void resetProccessList() //reset proccessHead/Tail to NULL
{
	proccessHead = proccessTail = NULL;
	Log_Event("Reset Proccess List.\n");
}

 void freeProccessMemory(proccessList* pList) //free proccess list memory
 {
	 dllList* currentDll;
	 dllList* releaseDll;
	 proccessList* currentProccess=pList;
	 proccessList* releaseProccess=pList;

	 Log_Event("Free Proccess memory.\n");

	 while (currentProccess != NULL)
	 {
		 currentDll = currentProccess->dList;
		 releaseDll = currentProccess->dList;

		 while (currentDll != NULL)
		 {
			 releaseDll = currentDll;
			 currentDll = currentDll->next;
			 free(releaseDll);
		 }
		 releaseProccess = currentProccess;
		 currentProccess = currentProccess->next;
		 free(releaseProccess);
	 }
 }

 //search if proccess item is in the search list by its name and id
 int searchProccessByNameAndId(char Name[1000], DWORD proccessId, proccessList* search)
 {
	 proccessList* curr = search;

	 if (curr == NULL) //list is empty- not found- return 1
	 {
		 return 1;
	 }

	 while (curr != NULL)
	 {
		 if (strcmp(curr->processName, Name) == 0 && proccessId==curr->proccessId)
		 {
			 return 0; //found- return 0
		 }
		 curr = curr->next;
	 }
	 return 1; //not found- return 1
 }

 //sum proccess lists. equal proccesses- the values and new dll will be added. new proccesse- will be added to end of list 
 proccessList* SumProccesses(proccessList* ret, proccessList* add)
 {
	 proccessList* currentProccess; 
	 proccessList* retTail = ret;
	 proccessList* retHead = ret; //head of return list
	 proccessList* cur;
	 dllList* curr;
	 dllList* retDll;
	 dllList* addDll;
	 dllList* newProccesDll;
	 dllList* newProccesDllList=NULL;

	 if (!ret)
	 {
		 return add;
	 }/*
	 while (retHead->prev != NULL)
	 {
		 retHead = retHead->prev;
	 }*/
	 while (retTail->next != NULL) 
	 {
		 retTail = retTail->next; //tail of return list
	 }

	 while (add != NULL) //added list
	 {
		 currentProccess = retHead; //return list

		 //if new proccess found- proccess that exist in add list but not ret list
		 if (searchProccessByNameAndId(add->processName, add->proccessId, currentProccess) != 0) 
		 {
			 cur = (proccessList*)malloc(sizeof(proccessList)); //memory allocation
			 if (!cur) //allocation check
			 {
				 Log_Error(strerror(GetLastError()));
				 return;
			 }
			 cur->next = NULL;
			 cur->prev = NULL;

			 //copy values to item
			 strcpy(cur->processName, add->processName);
			 cur->proccessId = add->proccessId;
			 cur->PageFaultCount = add->PageFaultCount;
			 cur->WorkingSetSize = add->WorkingSetSize;
			 cur->QuotaPagedPoolUsage = add->QuotaPagedPoolUsage;
			 cur->QuotaPeakPagedPoolUsage = add->QuotaPeakPagedPoolUsage;
			 cur->PagefileUsage = add->PagefileUsage;
			 cur->dllCnt = add->dllCnt;

			 //build dll List for the new item 
			 addDll = add->dList; // dll list of proccess item in add list
			 while (addDll != NULL)
			 {
				 newProccesDll = (dllList*)malloc(sizeof(dllList)); // memory allocation
				 if (!newProccesDll) // allocation check
				 {
					 Log_Error(strerror(GetLastError()));
					 return;
				 }
				 strcpy(newProccesDll->dllName, addDll->dllName); // copy values to item
				 newProccesDll->next = NULL;
				 newProccesDll->prev = NULL;

				 if (newProccesDllList == NULL) //if list is empty
				 {
					 newProccesDllList = newProccesDll;
				 }
				 else { //if not empty
					 while (newProccesDllList->next != NULL) //get pointer to last item in list
					 {
						 newProccesDllList = newProccesDllList->next;
					 }

					 
					 newProccesDllList->next = newProccesDll;
					 newProccesDll->prev = newProccesDllList;
					 newProccesDllList = newProccesDll;
				 }

				 /*while (newProccesDllList->prev != NULL)
				 {
					 newProccesDllList = newProccesDllList->prev;
				 }*/

				 addDll = addDll->next; //next dll item in dll list in proccess item in add list
			 }
			 if (newProccesDllList != NULL) 
			 {
				 while (newProccesDllList->prev != NULL) //get pointer to the head of the new created dll list
				 {
					 newProccesDllList = newProccesDllList->prev;
				 }
			 }
			 cur->dList = newProccesDllList; //put dll list in the new proccess item
			 newProccesDllList = NULL; //reset the pointer to NULL

			 if (retHead == NULL) //if return list is empty
			 {
				 retHead = cur;
				 retTail = cur;
			 }
			 else //if return list is not empty
			 {
				 retTail->next = cur;
				 cur->prev = retTail;
				 retTail = cur;
			 }
			 currentProccess = retHead; //reset pointer to the head of return list
		 }
		 else //if new proccess not found- the current add's proccess item exist in ret list
		 {
			 while (currentProccess != NULL)  
			 {
				 //search for the match proccesses
				 if (strcmp(currentProccess->processName, add->processName) == 0 && currentProccess->proccessId == add->proccessId)
				 {
					 retDll = currentProccess->dList;
					 addDll = add->dList;

					 while (addDll != NULL) //search if new dll exist in the add's proccess item but not in ret
					 {
						 if (searchDllByName(addDll->dllName, retDll) != 0)
						 {
							 currentProccess->dllCnt++; //upgrage dllCnt by one
							 curr = (dllList*)malloc(sizeof(dllList)); //memory allocation
							 if (!curr) // allocation check
							 {
								 Log_Error(strerror(GetLastError()));
								 return;
							 }
							 strcpy(curr->dllName, addDll->dllName); //copy name
							 curr->next = NULL;
							 curr->prev = NULL;

							 if (retDll == NULL) //if dll list of the ret's proccess item is empty
							 {
								 retDll = curr;
							 }
							 else { //if dll list of the ret's proccess item is not empty
								 while (retDll->next != NULL) //get the pointet to tail of dll list
								 {
									 retDll = retDll->next;
								 }

								 retDll->next = curr;
								 curr->prev = retDll;
								 retDll = curr;
							 }

							 while (retDll->prev != NULL) //get the pointet to head of dll list
							 {
								 retDll = retDll->prev;
							 }
						 }
						 addDll = addDll->next; //next dll item in dll list in proccess item in add list
					 }

					 //copy values of match proccesses 
					 currentProccess->PageFaultCount += add->PageFaultCount;
					 currentProccess->WorkingSetSize += add->WorkingSetSize;
					 currentProccess->QuotaPagedPoolUsage += add->QuotaPagedPoolUsage;
					 currentProccess->QuotaPeakPagedPoolUsage += add->QuotaPeakPagedPoolUsage;
					 currentProccess->PagefileUsage += add->PagefileUsage;
				 } 
				 currentProccess = currentProccess->next; //next proccess item of ret list
			 }	
			 currentProccess = retHead; //reset pointer to the head of return list
		 }
		 add = add->next; //next proccess item of add list
	 }

	 Log_Event("Sum Proccesses.\n");
	 return retHead;
 }

 unsigned short getProccessCnt(proccessList* pList) //count proccesses in plist
 {
	 Log_Event("Counting Proccesses.\n");

	 unsigned short proccessCnt = 0;
	 proccessList* currentProccess = pList;

	 while (currentProccess != NULL)
	 {
		 proccessCnt++;
		 currentProccess = currentProccess->next;
	 }

	 return proccessCnt;
 }

 void  freeProccessMemoryButNotHisDllList(proccessList* pList) //free proccess list memory but not his dll list
 {
	 proccessList* currentProccess = pList;
	 proccessList* releaseProccess = pList;

	 Log_Event("Free Proccess memory.\n");

	 while (currentProccess != NULL)
	 {
		 releaseProccess = currentProccess;
		 currentProccess = currentProccess->next;
		 free(releaseProccess);
	 }
 }

 //remove proccess multiplies in proccess list
 proccessList* removeProccessMultiplies(proccessList* pList)
 {
	 proccessList* dictionary = NULL;
	 proccessList* currentProccess = pList;
	 proccessList* curr;

	 while (currentProccess != NULL)
	 {
		 //search if proccess exist by name and id in dictionary- add if not 
		 if (searchProccessByNameAndId(currentProccess->processName, currentProccess->proccessId, dictionary) != 0)
		 {
			 curr = (proccessList*)malloc(sizeof(proccessList)); //memory allocation
			 if (!curr) //allocation check
			 {
				 Log_Error(strerror(GetLastError()));   //log
				 printf("Error of memory\n");
				 return;
			 }
			 curr->next = NULL;
			 curr->prev = NULL;

			 //copy values
			 strcpy(curr->processName, currentProccess->processName);
			 curr->proccessId = currentProccess->proccessId;
			 curr->PageFaultCount = currentProccess->PageFaultCount;
			 curr->WorkingSetSize = currentProccess->WorkingSetSize;
			 curr->QuotaPagedPoolUsage = currentProccess->QuotaPagedPoolUsage;
			 curr->QuotaPeakPagedPoolUsage = currentProccess->QuotaPeakPagedPoolUsage;
			 curr->PagefileUsage = currentProccess->PagefileUsage;
			 //curr->dList = currentProccess->dList;
			 curr->dllCnt = currentProccess->dllCnt;

			 if (dictionary == NULL) //if list is empty
			 {
				 dictionary = curr;
			 }
			 else { //if not empty
				 while (dictionary->next != NULL) //get pointer to tail of list
				 {
					 dictionary = dictionary->next;
				 }

				 dictionary->next = curr;
				 curr->prev = dictionary;
				 dictionary = curr;
			 }

			 while (dictionary->prev != NULL) //get pointer to head of list
			 {
				 dictionary = dictionary->prev;
			 }
		 }
		 currentProccess = currentProccess->next;
	 }

	 Log_Event("Proccess multiplies removed.\n");

	 return dictionary;
 }