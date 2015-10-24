#pragma once

//Database File
#define DATA_FILE "Data.dat"
#define DB_FILE "DB.dat"
#define INDEX_FILE "Index.dat"
#define DICTIONARY_FILE "Dictionary.dat"

//DB.dat
#define MAX_PAGE_NUM 32*1024
#define SIZE_PER_PAGE 4*1024

struct DB_FILE_Header
{
	char bitMap[MAX_PAGE_NUM / 8];
};

//Buffer
#define BUFFER_SIZE 2000

//Convert from int to bytes (the size of DB file can't be larger than 4GB because we use unsigned int as the file pointer)
void intToBytes(const char * data, char * result);
int power2(int times);
void setBitMap(char * bitMap, long position, bool flag);