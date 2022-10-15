void generateHtml(sampleList* sList); //generate html site

SIZE_T getMemoryAvg(sampleList* sList); //average memory for each sample item

char* createLink(char arr[10], unsigned short cnt); //create link to page

//create sorted sample html link to page
char* createSortSampleHtmlLink(unsigned char cnt);

//create sample html page- show sample number, time and it's proccess list
void createSampleHtml(sampleList* sList, char sampleHtmlName[20]); 

SIZE_T highestMemory(proccessList* pList); //get highest memory by workingSetSize value

//create sorted sample html page- show sample number, time and it's proccess list sorted by dll cnt
void createSortSampleHtml(proccessList* pList, char sampleHtmlName[20], char sortSampleHtmlName[30]);

//create dll html page- show number of proccess used this dll, dll name and proccess list
void createDllHtml(sampleList* sList, char dllHtmlName[20], char dllName[500]);