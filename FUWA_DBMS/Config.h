#pragma once

//Database File
#define DATA_FILE "Data.dat"
#define DB_FILE "DB.dat"
#define INDEX_FILE "Index.dat"
#define DICTIONARY_FILE "Dictionary.dat"

//DB.dat
#define MAX_PAGE_NUM 32*1024
#define PAGE_SIZE 4*1024

struct DB_FILE_Header
{
	char bitMap[MAX_PAGE_NUM / 8];
};

//Buffer
#define BUFFER_SIZE 2*1024

//Convert from int to bytes (the size of DB file can't be larger than 4GB because we use unsigned int as the file pointer)
void intToBytes(int intNumber, char * result);
int power2(int times);
void setBitMap(char * bitMap, long position, bool flag);
long getPhysicalAddress(int pageNumber, int pagePosition);
int getPageNumber(long physicalAddress);
int getPagePosition(long physicalAddress);
int getFreeSpaceInCurrentPage(long physicalAddress);