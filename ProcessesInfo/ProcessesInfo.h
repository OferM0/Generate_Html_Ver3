void getProccessName(TCHAR Buffer[MAX_PATH], char name[1000]); //get proccess name by convert buffer to char array

void getDllName(TCHAR Buffer[MAX_PATH], char name[500]); //get dll name by convert buffer to char array

proccessList* PrintMemoryInfo(DWORD processID); // For each Process to get its Memory Information

proccessList* GetProcessesInfo();
