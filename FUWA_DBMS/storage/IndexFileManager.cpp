#include "IndexFileManager.h"
#include "../Config.h"

IndexFileManager * IndexFileManager::indexFileManager = 0;

IndexFileManager * IndexFileManager::getInstance()
{
	if (indexFileManager == 0)
	{
		indexFileManager = new IndexFileManager(INDEX_FILE);
	}
	return indexFileManager;
}

IndexFileManager::IndexFileManager(string fileName) : DataFileManager(fileName)
{
}

IndexFileManager::~IndexFileManager()
{
}
