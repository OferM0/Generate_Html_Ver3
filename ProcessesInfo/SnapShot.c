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
#include "log.h"
#pragma warning(disable:4996)

unsigned short sampleCnt; //sample items counter

sampleList* snapShot() //create one sample item
{
	proccessList* pList;
	sampleList* sList;
	time_t t;
	char sampleTime[100];
	struct tm* timeinfo;

	time(&t);
	timeinfo = localtime(&t); //current time

	sprintf(sampleTime, "%d.%d.%d %d:%d:%d\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	pList = GetProcessesInfo(); //create proccess list
	resetProccessList(); //reset proccessHead/Tail to NULL
	sampleCnt++; //upgrage sample items counter by one
	sList=addSampleItem(pList, getProccessCnt(pList), sampleTime, sampleCnt); //add sample item to sList
	Log_Event("One SnapShots have been added.\n");

	return sList;
}