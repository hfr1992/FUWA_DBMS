#include "DataFileManager.h"
#include "../Config.h"

DataFileManager * DataFileManager::dataFileManager = 0;

DataFileManager::DataFileManager(string fileName)
{
	if (access(fileName.c_str(), 0)!=0)
	{
		fclose(fopen(fileName.c_str(), "w"));
	}
	fp = fopen(fileName.c_str(), "r+b");
}

DataFileManager::~DataFileManager()
{
	fclose(fp);
}

DataFileManager * DataFileManager::getInstance()
{
	if (dataFileManager == 0)
	{
		dataFileManager = new DataFileManager(DATA_FILE);
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
