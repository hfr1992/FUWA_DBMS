#include "IndexFileManager.h"

IndexFileManager * IndexFileManager::indexFileManager = 0;

IndexFileManager * IndexFileManager::getInstance()
{
	if (indexFileManager == 0)
	{
		indexFileManager = new IndexFileManager(INDEX_FILE);
	}
	return indexFileManager;
}

void IndexFileManager::deleteIndexFile()
{
	_unlink(INDEX_FILE);
	indexFileManager = 0;
	getInstance();
}

void IndexFileManager::appendBytes(void * object, long length)
{
	//char* temp = (char*)malloc(length*sizeof(char));
	append((char*)object, length);
}

void IndexFileManager::read(void * data, long from, long size)
{
	DataFileManager::read((char*)data, from, size);
}

long IndexFileManager::length()
{
	return DataFileManager::length();
}

//void IndexFileManager::writeIndex(BPlusTree * bPlusTree)
//{
//}
//
//void IndexFileManager::readIndex(BPlusTree * bPlusTree)
//{
//}

IndexFileManager::IndexFileManager(string fileName) : DataFileManager(fileName)
{
}

IndexFileManager::~IndexFileManager()
{
}
