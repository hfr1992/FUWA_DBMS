#include "DataFileManager.h"
#include "../Config.h"

DataFileManager * DataFileManager::dataFileManager = 0;

DataFileManager::DataFileManager()
{
	if (access(DATA_FILE, 0)!=0) 
	{
		fclose(fopen(DATA_FILE, "w"));
	}
	fp = fopen(DATA_FILE, "r+");
}

DataFileManager::~DataFileManager()
{
	fclose(fp);
}

DataFileManager * DataFileManager::getInstance()
{
	if (dataFileManager == 0)
	{
		dataFileManager = new DataFileManager();
	}
	return dataFileManager;
}

void DataFileManager::setPointer(long offset, int fromwhere)
{
	fseek(fp, offset, fromwhere);
}

void DataFileManager::read(char * data, long from, long size)
{
	setPointer(from, SEEK_SET);
	fread(data, sizeof(char), size, fp);
}

void DataFileManager::insert(long from, const char * data)
{
	setPointer(from, SEEK_SET);
	fwrite(data, sizeof(char), strlen(data), fp);
}

void DataFileManager::append(const char * data)
{
	setPointer(0, SEEK_END);
	fwrite(data, sizeof(char), strlen(data), fp);
}

long DataFileManager::length()
{
	setPointer(0, SEEK_END);
	return ftell(fp);
}
