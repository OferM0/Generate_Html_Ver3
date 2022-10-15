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
#include "log.h"
#pragma warning(disable:4996)

sampleList* TwentySnapShot() //create one sample item combined from 20 procceses lists
{
	proccessList* pList=NULL; //first proccess list
	proccessList* addList=NULL; //added proccess lists
	sampleList* sList;
	time_t t;
	char sampleTime[100];
	struct tm* timeinfo;

	time(&t);
	timeinfo = localtime(&t); //current time
	Log_Event("Start adding 20 SnapShots.\n");
	sprintf(sampleTime, "%d.%d.%d %d:%d:%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	for (int i = 0; i < 20; i++)
	{
		if (i == 0) //create first proccess list
		{
			pList = GetProcessesInfo();
			resetProccessList(); //reset proccessHead/Tail to NULL
		}
		else {
			addList = GetProcessesInfo(); //create added proccess lists
			resetProccessList(); //reset proccessHead/Tail to NULL
			pList=SumProccesses(pList, addList); //sum proccess lists (add addList to pList)
			freeProccessMemory(addList); //free addList memory after adding it values to pList
		}
		Sleep(1000); //delay one second
	}

	sampleCnt++; //upgrage sample items counter by one
	sList = addSampleItem(pList, getProccessCnt(pList), sampleTime, sampleCnt); //add sample item  
	Log_Event("20 SnapShots have been added.\n");

	return sList;
}