#include "DBFileManager.h"
#include "../Config.h"

DBFileManager * DBFileManager::dBFileManager = 0;

DBFileManager * DBFileManager::getInstance()
{
	if (dBFileManager == 0)
	{
		dBFileManager = new DBFileManager(DB_FILE);
	}
	return dBFileManager;
}

void DBFileManager::read(char * data, long from, long size)
{
	DataFileManager::read(data, from, size);
}

void DBFileManager::insert(long from, const char * data, int length)
{
	char int_Bytes[4] = { '\0' };
	intToBytes(data, int_Bytes);
	DataFileManager::insert(from, int_Bytes, 4);
	DataFileManager::insert(from + 4, data, length);
}

void DBFileManager::append(const char * data, int length)
{
	char int_Bytes[4] = { '\0' };
	intToBytes(data, int_Bytes);
	DataFileManager::append(int_Bytes,4);
	DataFileManager::append(data, length);
}

long DBFileManager::length()
{
	return DataFileManager::length();
}

DBFileManager::DBFileManager(string fileName) : DataFileManager(fileName)
{
}

DBFileManager::~DBFileManager()
{
}
