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
#include "20SnapShot.h"
#include "LongSnapShot.h"
#include "binaryFIiles.h"
#include "log.h"
#pragma warning(disable:4996)

proccessList* headProcess=NULL;
proccessList* tailProcess=NULL;

t_headerFile fileHeader; //fileHeader struct- give information how to save and load lists from files

void saveInFile(t_headerFile fileHeader, sampleList* head) //save sample list in binary file
{
	char fileName[100];
	sampleList* curr = head;
	proccessList* currProcess;
	dllList* currDll;

	if (!curr) //if list is empty there's no need to save, send message to user
	{
		Log_Event("The List is empty.\n");
		printf("The List is empty, Please take SnapShots first.\n");
		printf("----------------------------------------------------------------------\n");
		return;
	}

	printf("Enter file name: \n"); //get file name from user
	scanf("%s", fileName);
	FILE* f = fopen(fileName, "wb"); //open file to write
	if (!f) //file check
	{
		Log_Error(strerror(GetLastError()));
		return;
	}

	fwrite(&fileHeader, sizeof(t_headerFile), 1, f); //write fileHeader struct first

	for (int i = 0; i < fileHeader.itemsCount; i++) //fileHeader.itemsCount= number of items in sample list
	{
		fwrite(curr, sizeof(sampleList), 1, f); //write sample item
		
		currProcess = curr->pList;
		while (currProcess)
		{			
			fwrite(currProcess, sizeof(proccessList), 1, f); //write proccess  item of current sample item

			currDll = currProcess->dList;
			while (currDll)
			{
				fwrite(currDll, sizeof(dllList), 1, f); //write dll item of current proccess iteem of current sample item
				currDll = currDll->next;
			}

			currProcess = currProcess->next;
		}
		
		curr = curr->next;
	}
	Log_Event("The List saved in the file.\n");

	fclose(f); //close file
}

sampleList* LoadFromFile(t_headerFile fileHeader) //load sample list from file and return it
{
	resetDllList(); //reset dllHead/Tail to NULL
	resetProccessList(); //reset proccessHead/Tail to NULL

	sampleList* sList = NULL;
	proccessList* headProccess = NULL;
	dllList* headDll = NULL;

	sampleList* curr;
	proccessList* currProccess;
	dllList* currDll;

	char fileName[100];
	printf("Enter file name: \n"); //get file name from user
	scanf("%s", fileName);
	FILE* f = fopen(fileName, "rb"); //open file to read

	if (!f) //file check- if file not exist there's no need to load, send message to user
	{
		Log_Error(strerror(GetLastError()));
		printf("File error, Please give correct file name.\n");
		printf("----------------------------------------------------------------------\n");
		return;
	}

	fread(&fileHeader, sizeof(t_headerFile), 1, f); //read fileHeader struct first
	if (!fileHeader.itemsCount) //if fileHeader.itemsCount=0 it means there's nothing to read. empty list.
	{
		Log_Event("File is empty.");
		printf("The file is empty, Please choose another file. \n");
		return;
	}

	for (int i = 0; i < fileHeader.itemsCount; i++) //fileHeader.itemsCount= number of items in sample list to read
	{
		curr = (sampleList*)malloc(sizeof(sampleList)); //memory allocation
		if (!curr) //allocation check
		{
			Log_Error(strerror(GetLastError()));
			printf("Error in memory.\n");
			return;
		}
		fread(curr, sizeof(sampleList), 1, f); //read sample item

		currProccess = (proccessList*)malloc(sizeof(proccessList)); //memory allocation
		if (!currProccess) //allocation check
		{
			Log_Error(strerror(GetLastError()));
			printf("Error in memory.\n");
			return;
		}

		for (int l = 0; l < curr->proccessCnt; l++) //proccessCnt= number of proccess items in sample item to read
		{
			fread(currProccess, sizeof(proccessList), 1, f); //read proccess  item of current sample item

			currDll = (dllList*)malloc(sizeof(dllList)); //memory allocation
			if (!currDll) //allocation check
			{
				Log_Error(strerror(GetLastError()));
				printf("Error in memory.\n");
				return;
			}
		
			for (int j = 0; j < currProccess->dllCnt; j++) //dllCnt= number of dll items in proccess item in sample item to read
			{
				fread(currDll, sizeof(dllList), 1, f); //read dll item of current proccess iteem of current sample item
				headDll = addDllItem(currDll->dllName); //add dll item to dll list
				free(currDll); //free dll item memory

				currDll = (dllList*)malloc(sizeof(dllList)); //memory allocation
				if (!currDll) //allocation check
				{
					Log_Error(strerror(GetLastError()));
					printf("Error in memory.\n");
					return;
				}
			}

			free(currDll); //free dll item memory

			//add proccess item to proccess list
			headProccess = addProccessItem(currProccess->processName, currProccess->proccessId, currProccess->PageFaultCount, currProccess->WorkingSetSize, currProccess->QuotaPagedPoolUsage, currProccess->QuotaPeakPagedPoolUsage, currProccess->PagefileUsage, headDll, currProccess->dllCnt);
			resetDllList(); //reset dllHead/Tail to NULL
			free(currProccess); //free proccess item memory

			currProccess = (proccessList*)malloc(sizeof(proccessList)); //memory allocation
			if (!currProccess) //allocation check
			{
				Log_Error(strerror(GetLastError()));
				printf("Error in memory.\n");
				return;
			}
		}
		free(currProccess); //free proccess item memory

		sampleCnt++; //upgrage sample items counter by one
		//add sample item to sample list
		sList = addSampleItem(headProccess,curr->proccessCnt, curr->sampleTime, sampleCnt);
		resetProccessList(); //reset proccessHead/Tail to NULL
		free(curr); //free sample item memory
	}		
	Log_Event("The sample list of the file have been loaded and added to current list.\n");

	return sList;
}