void Log_Init(); //initialize the log file name by current time.

void Log(char message[200]); //get log message(event, error, warning) and create new message with current time and put the message in log file

void Log_Error(char message[200]); //get message and add Error

void Log_Event(char message[200]); //get message and add Event

void Log_Warning(char message[200]); //get message and add Warning
 
extern char file_name[200]; //global log file name, initialized at start of program by current time
