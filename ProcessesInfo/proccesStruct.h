typedef struct proccessList {

	//REAL DATA
	char processName[1000]; //proccess name
	DWORD proccessId; //proccess id

	//proccess memory data
	DWORD  PageFaultCount;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T PagefileUsage;

	//dll data
	dllList* dList;
	unsigned short dllCnt;

	//STRUCT MANAGMENT
	struct proccessList* next;
	struct proccessList* prev;
}proccessList;

//add proccess item to the list
proccessList* addProccessItem(char processName[1000], DWORD proccessId, DWORD  PageFaultCount, SIZE_T WorkingSetSize, SIZE_T QuotaPagedPoolUsage, SIZE_T QuotaPeakPagedPoolUsage, SIZE_T PagefileUsage, dllList* dList, unsigned short dllCnt);

void resetProccessList(); //reset proccessHead/Tail to NULL

//search if proccess item is in the search list by its name only
int searchProccessByName(char Name[1000], proccessList* search);

void  freeProccessMemory(proccessList* pList);  //free proccess list memory

 //search if proccess item is in the search list by its name and id
int searchProccessByNameAndId(char Name[1000], DWORD proccessId, proccessList* search);

//sum proccess lists. equal proccesses- the values and new dll will be added. new proccesse- will be added to end of list 
proccessList* SumProccesses(proccessList* ret, proccessList* add);

unsigned short getProccessCnt(proccessList* pList); //count proccesses in plist

void  freeProccessMemoryButNotHisDllList(proccessList * pList);  //free proccess list memory but not his dll list

proccessList* removeProccessMultiplies(proccessList* pList); //remove proccess multiplies in proccess list 



