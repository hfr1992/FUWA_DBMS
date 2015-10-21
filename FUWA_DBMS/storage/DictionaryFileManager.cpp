#include "DictionaryFileManager.h"
#include "../Config.h"

DictionaryFileManager * DictionaryFileManager::dictionaryFileManager = 0;

DictionaryFileManager * DictionaryFileManager::getInstance()
{
	if (dictionaryFileManager == 0)
	{
		dictionaryFileManager = new DictionaryFileManager(DICTIONARY_FILE);
	}
	return dictionaryFileManager;
}

DictionaryFileManager::DictionaryFileManager(string fileName) : DataFileManager(fileName)
{
}

DictionaryFileManager::~DictionaryFileManager()
{
}
