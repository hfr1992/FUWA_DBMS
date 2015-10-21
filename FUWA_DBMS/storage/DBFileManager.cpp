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

DBFileManager::DBFileManager(string fileName) : DataFileManager(fileName)
{
}

DBFileManager::~DBFileManager()
{
}
