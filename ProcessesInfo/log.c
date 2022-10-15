#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include "log.h"
#pragma warning(disable:4996)

char file_name[200]; //global log file name, initialized at start of program by current time

void Log_Init() //initialize the log file name by current time.
{
	time_t t;
	time(&t);
	struct tm* timeinfo;
	timeinfo = localtime(&t); //current time

	file_name[0] = '\0';
	sprintf(file_name, "%d_%d_%d_%d_%d_%d.log", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void Log(char message[200]) //get log message(event, error, warning) and create new message with current time and put the message in log file
{
	time_t t;
	time(&t);
	struct tm* timeinfo;
	timeinfo = localtime(&t); //current time

	char str[200];
	FILE* f = fopen(file_name, "a"); //open file to append

	//add current time to log message
	sprintf(str, "%d.%d.%d %d:%d:%d - %s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);
	fputs(str, f); //put the message in log file
	fclose(f); //close file
}

void Log_Error(char message[200]) //get message and add Error
{
	char str[200];
	sprintf(str, "%s %s", "Error - ", message);
	Log(str); //send message to write in file
}

void Log_Event(char message[200]) //get message and add Event
{
	char str[200];
	sprintf(str, "%s %s", "Event - ", message);
	Log(str); //send message to write in file
}

void Log_Warning(char message[200]) //get message and add Warning
{
	char str[200];
	sprintf(str, "%s %s", "Warning - ", message);
	Log(str); //send message to write in file
}