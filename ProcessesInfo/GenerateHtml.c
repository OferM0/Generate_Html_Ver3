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

void generateHtml(sampleList* sList) //generate html site
{
    sampleList* original = sList; //save head of list pointer
    proccessList* currentProccess;    
    dllList* currentDll;

    Log_Event("Start generate html pages.\n");

    if (sList != NULL)
    {
        currentProccess = sList->pList; //head of proccess list
    }
    else
    {
        currentProccess = NULL;
    }

    char html[10000]; 
    char value[100];
    char link[2000];

    FILE* writeHome = fopen("index.html", "w"); //open html file to write- home page
    // open static txt files to read
    FILE* readTopHome = fopen("index1.txt", "r"); 
    FILE* readMiddleHome = fopen("index2.txt", "r");
    FILE* readBottomHome = fopen("index3.txt", "r");

    //files check
    if ((!readTopHome) || (!readBottomHome) || (!readMiddleHome) || (!writeHome)) {
        Log_Error(strerror(GetLastError()));   //log
        printf("Error of open files, Please try again..\n");
        return;
    }
    else {
        while (fgets(html, sizeof(html), readTopHome)) //get what's write in file into array
        {
            fputs(html, writeHome); //write static file.
        }

        if (sList != NULL)
        {
            proccessList* fullProccess=NULL;
            proccessList* unMultipliedProccess=NULL;

            fullProccess = getFullProccessListBySampleList(sList); //create full conected proccess list of all proccess in all samples in sample list
            proccessList* originalForFree = fullProccess; //save head of the full proccess list to free later

            unMultipliedProccess = removeProccessMultiplies(fullProccess); //remove multiplies of full proccess list
            proccessList* originalForFree2 = unMultipliedProccess; //save head of the unMultiplied list to free later

            //get cnt of all different proccesses in whole sample list and write it in headline
            sprintf(value, " - %d different proccesses", getProccessCnt(unMultipliedProccess));

            freeProccessMemoryButNotHisDllList(originalForFree); //free the full proccess list that was created only for this dll page.
            freeProccessMemoryButNotHisDllList(originalForFree2); //free the unMultiplied proccess list that was created only for this dll page.

            fputs(value, writeHome); //write in headline number of all differnet proccesses in whole sample list
        }

        sList = original; //get slist pointer to it's head

        //write static html code
        fputs("</h2><table><tr><th>Id</th><th>Link</th><th>Proccess Cnt</th><th>Dll Cnt</th><th>Memory Avg</th></tr>", writeHome);
        
        while (sList != NULL)
        {
            //SAMPLE LIST
            fputs("<tr>", writeHome);
            fputs("<td>", writeHome);
            sprintf(value, "%d", sList->sampleCnt);
            fputs(value, writeHome); //1- sample number
            fputs("</td>", writeHome);
            fputs("<td>", writeHome);
            //create link by sample number
            sprintf(link, "%s%s%s%s%s", "<a class=\"link\" href=\"", createLink("sample", sList->sampleCnt), "\" target=\"_blank\">", createLink("sample", sList->sampleCnt), "</a>");
            fputs(link, writeHome); //2- sample html page link
            fputs("</td>", writeHome);
            sprintf(value, "%d", getProccessCnt(sList->pList)); 
            fputs("<td>", writeHome);
            fputs(value, writeHome); //3- write proccess cnt in sample item
            fputs("</td>", writeHome);
            fputs("<td>", writeHome);
            sprintf(value, "%d", getDllCnt(sList));
            fputs(value, writeHome); //4- write dll cnt in sample item
            fputs("</td>", writeHome);
            fputs("<td>", writeHome);
            sprintf(value, "%zu", getMemoryAvg(sList));
            fputs(value, writeHome); //5- write average memory of a proccess in sample item
            fputs("</td>", writeHome);
            fputs("</tr>", writeHome);

            //create sample html page by sample item and link
            createSampleHtml(sList, createLink("sample", sList->sampleCnt)); 
            sList = sList->next;
        }

        sList = original; //get slist pointer to it's head

        //write static html code
        fputs("</table></div><div class = \"right-content\"><h2>Dll's List", writeHome);

        if (sList != NULL)
        {
            //get cnt of all different dlls in whole sample list and write it in headline
            sprintf(value, " - %d different dll's", getDllCntByDllList(removeMultiplies(sList)));
            fputs(value, writeHome);
        }

        sList = original; //get slist pointer to it's head
        html[0]='\0'; //initiliaze array
        while (fgets(html, sizeof(html), readMiddleHome)) //get what's write in file into array
        {
            fputs(html, writeHome); //write static file.
        }

        /* all dll list without removing multiplies
        * 
        while (sList != NULL)
        {
            currentProccess = sList->pList;
            //DLL'S LIST
            while (currentProccess != NULL)
            {
                currentDll = currentProccess->dList;

                while (currentDll != NULL)
                {
                    fputs("<tr>", writeHome);
                    fputs("<td>", writeHome);
                    fputs(currentDll->dllName, writeHome); //1
                    fputs("</td>", writeHome);
                    fputs("<td>", writeHome);
                    fputs("link", writeHome); //2
                    fputs("</td>", writeHome);
                    fputs("</tr>", writeHome);

                    currentDll = currentDll->next;
                }
                currentProccess = currentProccess->next;
            }
            sList = sList->next;
        }*/

        
        if (sList != NULL)
        {
            // current dll= new dll list based on dictionary, removing multiplies.
            currentDll = removeMultiplies(sList); //create unMultiplied dll list
            dllList* originalDllForFree=currentDll;

            unsigned short dllNumber = 0; //use for link

            while (currentDll != NULL)
            {
                dllNumber++; //upgrade by one

                fputs("<tr>", writeHome);
                fputs("<td>", writeHome);
                fputs(currentDll->dllName, writeHome); //1- write dll name
                fputs("</td>", writeHome);
                fputs("<td>", writeHome);
                //create link by dll number
                sprintf(link, "%s%s%s%s%s", "<a class=\"link\" href=\"", createLink("dll", dllNumber), "\" target=\"_blank\">", createLink("dll", dllNumber), "</a>");
                fputs(link, writeHome); //2- dll html page link
                fputs("</td>", writeHome);
                fputs("</tr>", writeHome);

                //create dll html page by sample item, link and current dll item's name
                createDllHtml(sList, createLink("dll", dllNumber), currentDll->dllName);
                currentDll = currentDll->next;
            }
            freeDllMemory(originalDllForFree); //free dll unMultiplied list memory
        }

        html[0] = '\0'; //initiliaze array
        while (fgets(html, sizeof(html), readBottomHome)) //get what's write in file into array
        {
            fputs(html, writeHome); //write static file.
        }
    }
    Log_Event("Generated html pages.\n");

    //close files
    fclose(readTopHome);
    fclose(readMiddleHome);
    fclose(readBottomHome);
    fclose(writeHome);
}

//get average memory for each sample item
SIZE_T getMemoryAvg(sampleList* sList)
{
    SIZE_T memorySum = 0;
    unsigned short proccessCnt = 0;
    proccessList* currentProccess = sList->pList;
    Log_Event("Calculate all sample's avarage memory..\n");

    while (currentProccess != NULL)
    {
        proccessCnt++;
        //by workingsetsize value
        memorySum = memorySum + /*currentProccess->PageFaultCount + currentProccess->PagefileUsage + currentProccess->QuotaPagedPoolUsage + currentProccess->QuotaPeakPagedPoolUsage +*/ currentProccess->WorkingSetSize;
        currentProccess = currentProccess->next;
    }

    if (proccessCnt != 0) { memorySum = memorySum / proccessCnt; }

    return memorySum;
}

//create link to page
char* createLink(char arr [10], unsigned short cnt)
{
    char* link = (char*)malloc(sizeof(char)* 20);
    if (!link)
    {
        Log_Error(strerror(GetLastError()));
        printf("Error in creating link.\n");
        return;
    }
    sprintf(link, "%s%d.html", arr, cnt);
    Log_Event("Creating link\n");
    return link;
}

//create sorted sample html link to page
char* createSortSampleHtmlLink(unsigned char cnt)
{
    char* link = (char*)malloc(sizeof(char) * 30);
    if (!link)
    {
        Log_Error(strerror(GetLastError()));
        printf("Error in creating Sorted Sample link.\n");
        return;
    }
    sprintf(link, "sortSample%d.html", cnt);
    Log_Event("Creating Sorted Sample link.\n");
    return link;
}

//create sample html page- show sample number, time and it's proccess list
void createSampleHtml(sampleList* sList, char sampleHtmlName[20])
{
    proccessList* currentProccess = sList->pList;
    dllList* currentDll;

    char html[10000];
    char sampleInformation[30];
    char value[100];
    char sortSampleHtmlName[30];

    FILE* writeSample = fopen(sampleHtmlName, "w"); //open html file to write- sample page
    // open static txt files to read
    FILE* readSortBtn = fopen("sample0.txt", "r");
    FILE* readTopSample = fopen("sample1.txt", "r");
    FILE* readMiddleSample = fopen("sample2.txt", "r");
    FILE* readBottomSample = fopen("sample3.txt", "r");

    Log_Event("Creating Sample html page.\n");

    //files check
    if ((!readTopSample) || (!readBottomSample) || (!readMiddleSample) || (!readSortBtn) || (!writeSample)) {
        Log_Error(strerror(GetLastError()));   //log
        printf("Error of open files, Please try again..\n");
        return;
    }
    else {
        //get what's write in file into array
        while (fgets(html, sizeof(html), readSortBtn))
        {
            fputs(html, writeSample); //write static file.
        }

        strcpy(sortSampleHtmlName, createSortSampleHtmlLink(sList->sampleCnt));
        fputs(sortSampleHtmlName, writeSample); //write link to sorted sample page

        html[0] = '\0'; //initiliaze array
        //get what's write in file into array
        while (fgets(html, sizeof(html), readTopSample))
        {
            fputs(html, writeSample); //write link to sorted sample page
        }

        sprintf(sampleInformation, "%d at %s", sList->sampleCnt, sList->sampleTime);
        fputs(sampleInformation, writeSample); //write sample information- sample number, time

        html[0] = '\0'; //initiliaze array
        //get what's write in file into array
        while (fgets(html, sizeof(html), readMiddleSample))
        {
            fputs(html, writeSample); //write link to sorted sample page
        }

        while (currentProccess != NULL)
        {
            //PROCCESS LIST
            fputs("<tr>", writeSample);
            fputs("<td>", writeSample);
            fputs(currentProccess->processName, writeSample); //1- write proccess name
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->proccessId);
            fputs(value, writeSample); //2- write proccess id
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->PageFaultCount);
            fputs(value, writeSample); //3- write proccess's PageFaultCount
            fputs("</td>", writeSample);
            sprintf(value, "%zu", currentProccess->WorkingSetSize);
            if (highestMemory(sList->pList) == currentProccess->WorkingSetSize) // add icon
            {
                fputs("<td class=\"warning-icon\">", writeSample);
                fputs("<p>", writeSample);
                fputs(value, writeSample); //4- write proccess's WorkingSetSize with icon
                fputs("<img class=\"warning-icon\" src=\"images/icons8-high-priority-64.png\"/></p>", writeSample); 
            }
            else
            {
                fputs("<td>", writeSample);
                fputs(value, writeSample); //4- write proccess's WorkingSetSize
            }
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPagedPoolUsage);
            fputs(value, writeSample); //5- write proccess's QuotaPagedPoolUsage
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPeakPagedPoolUsage);
            fputs(value, writeSample); //6- write proccess's QuotaPeakPagedPoolUsage
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->PagefileUsage);
            fputs(value, writeSample); //7- write proccess's PagefileUsage
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->dllCnt);
            fputs(value, writeSample); //8- write proccess's dllCnt
            fputs("</td>", writeSample);

            fputs("<td>", writeSample); //write dll list in select look
            fputs("<div class = \"box\"><select>", writeSample);
            fputs("<option hidden class=\"option-h\">List</option>", writeSample);
            currentDll = currentProccess->dList;
            while (currentDll != NULL)
            {
                fputs("<option>", writeSample);
                fputs(currentDll->dllName, writeSample);
                fputs("</option>", writeSample);
                currentDll = currentDll->next;
            }
            fputs("</select></div>", writeSample);
            fputs("</td>", writeSample);
            fputs("</tr>", writeSample);

            currentProccess = currentProccess->next;
        }

        html[0] = '\0'; //initiliaze array
        //get what's write in file into array
        while (fgets(html, sizeof(html), readBottomSample))
        {
            fputs(html, writeSample); //write static file.
        }

        currentProccess = sList->pList;

        //create sorted sample html page by proccess list and links
        createSortSampleHtml(currentProccess, sampleHtmlName, sortSampleHtmlName);
    }
    Log_Event("Created Sample html page.\n");

    //close files
    fclose(readTopSample);
    fclose(readMiddleSample);
    fclose(readBottomSample);
    fclose(writeSample);
    fclose(readSortBtn);
}

//get highest memory by workingSetSize value
SIZE_T highestMemory(proccessList* pList)
{
    SIZE_T cmp = 0;
    proccessList* currentProccess = pList;
    Log_Event("Finding highest memory.\n");

    while (currentProccess != NULL)
    {
        if (currentProccess->WorkingSetSize > cmp) 
        {
            cmp = currentProccess->WorkingSetSize;
        }
        currentProccess = currentProccess->next;
    }

    return cmp;
}

//create sorted sample html page- show sample number, time and it's proccess list sorted by dll cnt
void createSortSampleHtml(proccessList* pList, char sampleHtmlName[20], char sortSampleHtmlName[30])
{
    SIZE_T mostMemory= highestMemory(pList);
    proccessList* original=pList;
    proccessList* currentProccess = makeCopySortedList(pList); //create copy and sorted proccess list
    proccessList* originalForFree = currentProccess; //save head of list to free later
    dllList* currentDll;

    char html[10000];
    char value[100];

    FILE* writeSample = fopen(sortSampleHtmlName, "w"); //open html file to write- sample page
    // open static txt files to read
    FILE* readSortBtn = fopen("sample0.txt", "r");
    FILE* readMiddleSample = fopen("sortSample1.txt", "r");
    FILE* readBottomSample = fopen("sample3.txt", "r");

    Log_Event("Creating Sorted Sample html page.\n");

    //files check
    if ((!readSortBtn) || (!readBottomSample) || (!readMiddleSample) || (!writeSample)) {
        Log_Error(strerror(GetLastError()));   //log
        printf("Error of open files, Please try again..\n");
        return;
    }
    else {
        //get what's write in file into array
        while (fgets(html, sizeof(html), readSortBtn))
        {
            fputs(html, writeSample); //write static file.
        }

        fputs(sampleHtmlName, writeSample); //write link to sample html page

        html[0] = '\0'; //initiliaze array
        //get what's write in file into array
        while (fgets(html, sizeof(html), readMiddleSample))
        {
            fputs(html, writeSample); //write link to sorted sample page
        }

        while (currentProccess != NULL)
        {
            //PROCCESS LIST
            fputs("<tr>", writeSample);
            fputs("<td>", writeSample);
            fputs(currentProccess->processName, writeSample); //1- write proccess name
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->proccessId);
            fputs(value, writeSample); //2- write proccess id
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->PageFaultCount);
            fputs(value, writeSample); //3- write proccess's PageFaultCount
            fputs("</td>", writeSample);
            sprintf(value, "%zu", currentProccess->WorkingSetSize);
            if (mostMemory == currentProccess->WorkingSetSize) // add icon
            {
                fputs("<td class=\"warning-icon\">", writeSample);
                fputs("<p>", writeSample);
                fputs(value, writeSample); //4- write proccess's WorkingSetSize with icon
                fputs("<img class=\"warning-icon\" src=\"images/icons8-high-priority-64.png\"/></p>", writeSample); 
            }
            else
            {
                fputs("<td>", writeSample);
                fputs(value, writeSample); //4- write proccess's WorkingSetSize with icon
            }
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPagedPoolUsage);
            fputs(value, writeSample); //5- write proccess's QuotaPagedPoolUsage
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->QuotaPeakPagedPoolUsage);
            fputs(value, writeSample); //6- write proccess's QuotaPeakPagedPoolUsage
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%zu", currentProccess->PagefileUsage);
            fputs(value, writeSample); //7- write proccess's PagefileUsage
            fputs("</td>", writeSample);
            fputs("<td>", writeSample);
            sprintf(value, "%d", currentProccess->dllCnt);
            fputs(value, writeSample); //8- write proccess's dllCnt
            fputs("</td>", writeSample);

            fputs("<td>", writeSample); //write dll list in select look
            fputs("<div class = \"box\"><select>", writeSample);
            fputs("<option hidden class=\"option-h\">List</option>", writeSample);
            currentDll = currentProccess->dList;
            while (currentDll != NULL)
            {
                fputs("<option>", writeSample);
                fputs(currentDll->dllName, writeSample);
                fputs("</option>", writeSample);
                currentDll = currentDll->next;
            }
            fputs("</select></div>", writeSample);
            fputs("</td>", writeSample);
            fputs("</tr>", writeSample);

            currentProccess = currentProccess->next;
        }

        freeProccessMemoryButNotHisDllList(originalForFree); //free the unMultiplied proccess list that was created only for this dll page.
        /*
        if (currentProccess != NULL)
        {
            //DLL'S LIST
            while (currentProccess != NULL)
            {
                currentDll = currentProccess->dList;

                while (currentDll != NULL)
                {
                    fputs("<tr>", writeSample);
                    fputs("<td>", writeSample);
                    fputs(currentDll->dllName, writeSample); //1
                    fputs("</td>", writeSample);
                    fputs("<td>", writeSample);
                    fputs("link", writeSample); //2
                    fputs("</td>", writeSample);
                    fputs("</tr>", writeSample);

                    currentDll = currentDll->next;
                }
                currentProccess = currentProccess->next;
            }
        }*/

        html[0] = '\0'; //initiliaze array
        //get what's write in file into array
        while (fgets(html, sizeof(html), readBottomSample))
        {
            fputs(html, writeSample); //write static file.
        }
    }
    Log_Event("Created Sorted Sample html page.\n");

    //close files
    fclose(readSortBtn);
    fclose(readMiddleSample);
    fclose(readBottomSample);
    fclose(writeSample);
}

//create dll html page- show number of proccess used this dll, dll name and proccess list
void createDllHtml(sampleList* sList, char dllHtmlName[20], char dllName[500])
{
    //create copy and unMultiplied proccess list
    proccessList* currentProccess = removeProccessMultiplies(getProccessListByDllName(sList, dllName));
    proccessList* originalForFree = currentProccess; //save head of list to free later
    dllList* currentDll;

    char html[10000];
    char value[300];

    FILE* writeDll = fopen(dllHtmlName, "w"); //open html file to write- sample page
    // open static txt files to read
    FILE* readTopDll = fopen("Dll1.txt", "r");
    FILE* readMiddleDll = fopen("Dll2.txt", "r");
    FILE* readBottomDll = fopen("Dll3.txt", "r");

    Log_Event("Creating Dll html page.\n");

    //files check
    if ((!readTopDll) || (!readBottomDll) || (!readMiddleDll) || (!writeDll)) {
        Log_Error(strerror(GetLastError()));   //log
        printf("Error of open files, Please try again..\n");
        return;
    }
    else {
        //get what's write in file into array
        while (fgets(html, sizeof(html), readTopDll))
        {
            fputs(html, writeDll); //write static file.
        }

        sprintf(value, "%d PROCCESSES USED %s", getProccessCnt(currentProccess), dllName);
        fputs(value, writeDll); //write information- how many proccesses used the dll and it's name

        html[0] = '\0';
        while (fgets(html, sizeof(html), readMiddleDll))
        {
            fputs(html, writeDll);
        }

        while (currentProccess != NULL)
        {
            //PROCCESS LIST
            fputs("<tr>", writeDll);
            fputs("<td>", writeDll);
            fputs(currentProccess->processName, writeDll); //1- write proccess name
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%d", currentProccess->proccessId);
            fputs(value, writeDll); //2- write proccess id
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%zu", currentProccess->PageFaultCount);
            fputs(value, writeDll); //3- write proccess's PageFaultCount
            fputs("</td>", writeDll);
            sprintf(value, "%zu", currentProccess->WorkingSetSize);
            fputs("<td>", writeDll);
            fputs(value, writeDll); //4- write proccess's WorkingSetSize
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%zu", currentProccess->QuotaPagedPoolUsage);
            fputs(value, writeDll); //5- write proccess's QuotaPagedPoolUsage
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%zu", currentProccess->QuotaPeakPagedPoolUsage);
            fputs(value, writeDll); //6- write proccess's QuotaPeakPagedPoolUsage
            fputs("</td>", writeDll);
            fputs("<td>", writeDll);
            sprintf(value, "%zu", currentProccess->PagefileUsage);
            fputs(value, writeDll); //7- write proccess's PagefileUsage
            fputs("</td>", writeDll);
            fputs("</tr>", writeDll);

            currentProccess = currentProccess->next;
        }

        freeProccessMemoryButNotHisDllList(originalForFree); //free the unMultiplied proccess list that was created only for this dll page.

        html[0] = '\0'; //initiliaze array
        //get what's write in file into array
        while (fgets(html, sizeof(html), readBottomDll))
        {
            fputs(html, writeDll); //write static file.
        }
    }
    Log_Event("Created Dll html page.\n");

    //close files
    fclose(readTopDll);
    fclose(readMiddleDll);
    fclose(readBottomDll);
    fclose(writeDll);
}
