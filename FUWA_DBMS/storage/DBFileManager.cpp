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

void DBFileManager::insert(const char * data, long from, int length)
{
	DataFileManager::insert(data, from, length);
}

void DBFileManager::append(const char * data, int length)
{
	DataFileManager::append(data, length);
}

long DBFileManager::length()
{
	return DataFileManager::length();
}

DB_FILE_Header * DBFileManager::getDBFileHeader()
{
	return (&dB_FILE_Header);
}

void DBFileManager::setDBFileHeader(DB_FILE_Header * Header)
{
	insert(Header->bitMap, 0, (MAX_PAGE_NUM / 8)*sizeof(char));
	memcpy(&dB_FILE_Header, Header, sizeof(DB_FILE_Header));
}

DBFileManager::DBFileManager(string fileName) : DataFileManager(fileName)
{
	if (length() == 0)
	{
		createBitMap();
	}
	else
	{
		getBitMap();
	}
}

DBFileManager::~DBFileManager()
{
}

void DBFileManager::getBitMap()
{
	read(dB_FILE_Header.bitMap, 0, MAX_PAGE_NUM / 8);
}

void DBFileManager::createBitMap()
{
	for (int i = 0; i < MAX_PAGE_NUM / 8; i++)
	{
		dB_FILE_Header.bitMap[i] = (char)(0x00);
	}
	insert(dB_FILE_Header.bitMap, 0, (MAX_PAGE_NUM / 8)*sizeof(char));
}
