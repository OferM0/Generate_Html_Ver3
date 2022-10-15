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
#include "log.h"
#pragma warning(disable:4996)

sampleList* LongSnapShot() //create one sample item combined from procceses lists(taken each second) based on user
{
	proccessList* pList = NULL; //first proccess list
	proccessList* addList = NULL; //added proccess lists
	sampleList* sList;
	time_t t;
	char sampleTime[100];
	struct tm* timeinfo;
	unsigned char firstProccess = 0;
	int Stop = 0;
	char ch1[10];

	time(&t);
	timeinfo = localtime(&t); //current time

	Log_Event("Starting Long SnapShots.\n");
	sprintf(sampleTime, "%d.%d.%d %d:%d:%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	printf("Click End to stop the SanpShot\n");
	while (!Stop) //while user not type End
	{
		if (firstProccess == 0) //create first proccess list
		{
			pList = GetProcessesInfo();
			resetProccessList(); //reset proccessHead/Tail to NULL
			firstProccess++;
		}
		else {
			addList = GetProcessesInfo(); //create added proccess lists
			resetProccessList(); //reset proccessHead/Tail to NULL
			pList = SumProccesses(pList, addList); //sum proccess lists (add addList to pList)
			freeProccessMemory(addList); //free addList memory after adding it values to pList
		}
		Sleep(1000); //delay one second
		if (kbhit()) //if user clicked on keyboard
		{
			gets(ch1); //get the string user typed 
			printf("The user clicked %s\n", ch1);
			if (strcmp(ch1,"End")==0) //if user types End
			{
				Log_Event("Ending Long SnapShots.\n");
				Stop = 1; //stop loop
			}
		}
	}
	printf("----------------------------------------------------------------------\n");

	sampleCnt++; //upgrage sample items counter by one
	sList = addSampleItem(pList, getProccessCnt(pList), sampleTime, sampleCnt); //add sample item
	Log_Event("Long SnapShots have been added.\n");

	return sList;
}


