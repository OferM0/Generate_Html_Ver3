typedef struct dllList {

	//REAL DATA
	char dllName[500]; //dll name

	//STRUCT MANAGMENT
	struct dllList* next;
	struct dllList* prev;
}dllList;

dllList* addDllItem(char dllName[500]); //add dll item to the list

int searchDllByName(char Name[500], dllList* search); //search if dll item is in the search list by its name

void resetDllList(); //reset dllHead/Tail to NULL

unsigned short getDllCntByDllList(dllList* search); //count dll in search list

void freeDllMemory(dllList* dList); //free dll list memory

