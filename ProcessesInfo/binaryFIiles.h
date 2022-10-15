typedef struct {
	int version;
	int itemsCount;
	char reserve[100];
} t_headerFile;

extern t_headerFile fileHeader; //fileHeader struct- give information how to save and load lists from files

void saveInFile(t_headerFile fileHeader, sampleList* head); //save sample list in binary file

sampleList* LoadFromFile(t_headerFile fileHeader); //load sample list from file and return it
