#include "TableManager.h"
#include <stdlib.h>
#include "BufferManager.h"

#define TEST_DATA_SIZE 2000

TableManager::TableManager()
{
	//struct Table_1 tuples[TEST_DATA_SIZE];
	//createTestData(tuples);
	//
	//int tupleSize = sizeof(int)+ sizeof(char)+ sizeof(char*);
	//int requireSize = TEST_DATA_SIZE * (tupleSize + sizeof(int) * 3);

	//char * target = (char *)malloc(requireSize);

	//char lengthNumber[4];

	//for (int i = 0; i < TEST_DATA_SIZE; i++)
	//{
	//	intToBytes(sizeof(int), lengthNumber);
	//	memcpy(target, lengthNumber, sizeof(int));
	//	target += sizeof(int);
	//	memcpy(target, &(tuples[i].col_int), sizeof(int));
	//	target += sizeof(int);

	//	intToBytes(sizeof(char), lengthNumber);
	//	memcpy(target, lengthNumber, sizeof(int));
	//	target += sizeof(int);
	//	memcpy(target, &(tuples[i].col_char), sizeof(char));
	//	target += sizeof(char);

	//	intToBytes(strlen(tuples[i].col_string)*sizeof(char), lengthNumber);
	//	memcpy(target, lengthNumber, sizeof(int));
	//	target += sizeof(int);
	//	memcpy( target, &(tuples[i].col_string), strlen(tuples[i].col_string)*sizeof(char) );
	//	target += strlen(tuples[i].col_string)*sizeof(char);
	//}

	//BufferManager * bufferManager = BufferManager::getInstance();
	//long freeSpace = bufferManager->findFreeSpace(requireSize);

	//deleteTestData(tuples);
}

TableManager::~TableManager()
{
}

void TableManager::insertOneTuple(char * tuple, int size)
{
	BufferManager * bufferManager = BufferManager::getInstance();
	long insertPosition = bufferManager->insertOneTuple(tuple, size);
	//to insert the record into index
}

void TableManager::selectOneTuple(char * tuple, int size, long position)
{
	BufferManager * bufferManager = BufferManager::getInstance();
	bufferManager->selectOneTuple(tuple, size, position);
}

void TableManager::deleteOneTuple(int size, long position)
{
	BufferManager * bufferManager = BufferManager::getInstance();
	bufferManager->deleteOneTuple(size, position);
	//to delete the record from index
}

//void TableManager::createTestData(Table_1 * tuples)
//{
//	for (int i = 0; i < TEST_DATA_SIZE; i++)
//	{
//		tuples[i].col_int = i;
//		tuples[i].col_char = (char)('A' + i % 26);
//
//		int strlen = (rand() % 100);
//		tuples[i].col_string = (char *)malloc(strlen*sizeof(char));
//		for (int j = 0; j < strlen; j++)
//		{
//			tuples[i].col_string[j] = (char)('A' + i % 26);
//		}
//		tuples[i].col_string[strlen - 1] = '\0';
//	}
//}
//
//void TableManager::deleteTestData(Table_1 * tuples)
//{
//	for (int i = 0; i < TEST_DATA_SIZE; i++)
//	{
//		free(tuples[i].col_string);
//	}
//}
