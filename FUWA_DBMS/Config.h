#pragma once

//Database File
#define DATA_FILE "Data.dat"
#define DB_FILE "DB.dat"
#define INDEX_FILE "Index.dat"
#define DICTIONARY_FILE "Dictionary.dat"

//DB.dat
#define MAX_PAGE_NUM 40000
#define SIZE_PER_PAGE 4*1024

//Convert from int to bytes (the size of DB file can't be larger than 4GB because we use unsigned int as the file pointer)
void intToBytes(const char * data, char * result);