#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <windows.h>
#include <psapi.h>

#include "dllStruct.h"
#include "proccesStruct.h"
#include "sampleStruct.h"
#include "ProcessesInfo.h"
#include "log.h"
#pragma warning(disable:4996)

void getProccessName(TCHAR Buffer[MAX_PATH], char name[1000]) //get proccess name by convert buffer to char array
{
	//Log_Event("getting proccess name.\n");
	char proccessName[1000];
	unsigned short determineChar;

	for (int i = 0; Buffer[i] != '\0'; i++) 
	{
		proccessName[i] = Buffer[i];
		determineChar = i + 1;
	}
	proccessName[determineChar] = '\0';

	/* to get tne name itself " ".exe
	
	for (int j = 0; proccessName[j] != '\0'; j++)
	{
		if (proccessName[j] == '\\') {
			determineChar = j+1;
		}
	}
	strcpy(proccessName, &proccessName[determineChar]);
	*/

	strcpy(name, proccessName);
}

void getDllName(TCHAR Buffer[MAX_PATH], char name[500]) //get dll name by convert buffer to char array
{
	//Log_Event("getting dll name.\n");
	char dllName[500];
	unsigned short determineChar;

	for (int i = 0; Buffer[i] != '\0'; i++)
	{
		dllName[i] = Buffer[i];
		determineChar = i + 1;
	}
	dllName[determineChar] = '\0';

	/* to get tne name itself " ".dll

	for (int j = 0; dllName[j] != '\0'; j++)
	{
		if (dllName[j] == '\\') {
			determineChar = j + 1;
		}
	}
	strcpy(dllName, &dllName[determineChar]);
*/
	strcpy(name, dllName);
}

proccessList* PrintMemoryInfo(DWORD processID)
{
	proccessList* pList=NULL;
	char processName[1000], dllName[500], checkFirstDllEqualBuffer = 0;
	dllList* dlList=NULL;
	unsigned short dllCnt = 0;

	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Open process in order to receive information
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
	{
		// Write to log a warning
		return;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR Buffer[MAX_PATH]; //proccess name
	TCHAR Buffer2[MAX_PATH]; //dll name

	// Get Process Name into buffer
	if (GetModuleFileNameEx(hProcess, 0, Buffer, MAX_PATH))
	{
		// At this point, buffer contains the full path to the executable
		getProccessName(Buffer, processName); //convert buffer to char array

		if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
		{
			// Get Dlls List
			if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
			{
				for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
				{
					TCHAR szModName[MAX_PATH];

					// Get dll Name into buffer
					if (GetModuleFileNameEx(hProcess, hMods[i], Buffer2, MAX_PATH))
					{
						getDllName(Buffer2, dllName); //convert buffer to char array
						if (checkFirstDllEqualBuffer != 0) //first one is the not dll but proccess name
						{
							dlList = addDllItem(dllName); //add dll item to list
							dllCnt++; //upgrade dll counter by one
						}
						checkFirstDllEqualBuffer = 1;
					}
				}
			}
			//Log_Event("getting proccess memory info.\n");

			//add proccess item to list
			pList = addProccessItem(processName, processID, pmc.PageFaultCount, pmc.WorkingSetSize, pmc.QuotaPagedPoolUsage, pmc.QuotaPeakPagedPoolUsage, pmc.PagefileUsage, dlList, dllCnt);
			resetDllList(); //reset dllHead/Tail to NULL
			dlList = NULL;
		}
	}
	else // if cant get process Name into buffer
	{
		Log_Error(strerror(GetLastError()));
	}
	CloseHandle(hProcess);
	return pList;
}

proccessList* GetProcessesInfo()
{
	proccessList* pList=NULL;

	// Get Processes
	// Receive all process ID
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	// * Receive all process ID and put in aProcesses Array
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		Log_Error(strerror(GetLastError()));
		printf("Error open proccesses(!EnumProcesses).\n");
		return 1;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the memory usage for each process
	// *Loop of all processes
	for (i = 0; i < cProcesses; i++)
	{
		pList = PrintMemoryInfo(aProcesses[i]); // For each Process to get its Memory Information
	}

	
	return pList;
}