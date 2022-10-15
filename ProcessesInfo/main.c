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
#define Version 1

int main()
{
	Log_Init(); //initialize the log file name by current time.
	Log_Event("Start of program.\n");
	char userResponse;
	sampleList* sList=NULL;
	fileHeader.version = 1;
	sampleCnt = 0;

	// user menu
	printf("Dear user, please choose what to do:\n\n1- Take One SnapShot.\n2- Take 20 SnapShots.\n3- Start Long SnapShot.\n4- Generate HTML Reports.\n5- Reset Collections.\n6- Save in file.\n7- Load from file.\n8-Quit.\n\n");
	scanf(" %c", &userResponse);
	printf("----------------------------------------------------------------------\n");
	while (userResponse != '8')
	{
		switch (userResponse)
		{
		case '1':
			sList = snapShot(); //take one snapshots
			break;

		case '2':
			sList = TwentySnapShot(); //take twenty snapshots
			break;

		case '3':
			sList = LongSnapShot(); //take long snapshots untill user click End
			break;

		case '4': //Generate HTML Reports
			if (!sList) //if sList is empty(no snapshots taken) the user will get a warning message
			{
				printf("Sample list is empty, Please take SnapShots first.\n");
				printf("----------------------------------------------------------------------\n");
			}
			else {
				generateHtml(sList);
			}
			break;

		case '5': //Reset collections
			resetCollections(); //free memory of sample list
			resetSampleList(); //initialize  sampleHead/Tail to NULL
			sList = NULL; //initialize  sList to NULL
			generateHtml(sList); //initialize  html pages to static look.
			sampleCnt = 0;
			break;

		case '6': //save sample list in file
				fileHeader.itemsCount = sampleCnt; //change itemsCount of the fileHeader to the sampleCnt
				saveInFile(fileHeader, sList);
			break;

		case '7':
			sList = LoadFromFile(fileHeader); //load from file
			break;

		case '8':

			break;

		default: //if user click wrong.
			printf("\nWrong selection, Try to be more sharp.\n");
			break;
		}

		printf("\nDear user, please choose what to do:\n\n1- Take One SnapShot.\n2- Take 20 SnapShots.\n3- Start Long SnapShot.\n4- Generate HTML Reports.\n5- Reset Collections.\n6- Save in file.\n7- Load from file.\n8-Quit.\n\n");
		scanf(" %c", &userResponse);
		printf("----------------------------------------------------------------------\n");
	}
	printf("Thank you for using our service. See you next time.\n");
	Log_Event("End of program.");

	return 0;
}

int main1()
{
	Log_Init();

	//sampleList* sList = NULL;
	proccessList* pList;

	pList = GetProcessesInfo();
	//sList=snapShot();

	return 0;
}