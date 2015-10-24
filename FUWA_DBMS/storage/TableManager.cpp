#include "TableManager.h"
#include <stdlib.h>

#define TEST_DATA_SIZE 2000

TableManager::TableManager()
{
	struct Table_1 tuples[TEST_DATA_SIZE];
	createTestData(tuples);
	//Unfinished

	deleteTestData(tuples);
}

TableManager::~TableManager()
{
}

void TableManager::createTestData(Table_1 * tuples)
{
	for (int i = 0; i < TEST_DATA_SIZE; i++)
	{
		tuples[i].col_int = i;
		tuples[i].col_char = (char)('A' + i % 26);

		int strlen = (rand() % 100);
		tuples[i].col_string = (char *)malloc(strlen*sizeof(char));
		for (int j = 0; j < strlen; j++)
		{
			tuples[i].col_string[j] = (char)('A' + i % 26);
		}
		tuples[i].col_string[strlen - 1] = '\0';
	}
}

void TableManager::deleteTestData(Table_1 * tuples)
{
	for (int i = 0; i < TEST_DATA_SIZE; i++)
	{
		free(tuples[i].col_string);
	}
}
