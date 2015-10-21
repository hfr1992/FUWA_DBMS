// FUWA_DBMS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "storage/IndexFileManager.h"
using namespace std;

void testDataFileManager() 
{
	cout << "Testing DataFileManager..." << endl;

	DataFileManager * dataFileManager = DataFileManager::getInstance();

	dataFileManager->append("Hello, Data!");
	dataFileManager->insert(5, "**");

	char temp[10] = {'\0'};
	dataFileManager->read(temp, 0, 9);
	cout << temp << endl;

	cout << dataFileManager->length() << endl;

	cout << "DataFileManager Test Finished." << endl;
}

void testIndexFileManager()
{
	cout << "Testing IndexFileManager..." << endl;

	IndexFileManager * indexFileManager = IndexFileManager::getInstance();

	indexFileManager->append("Hello, Index!");
	indexFileManager->insert(5, "**");

	char temp[10] = { '\0' };
	indexFileManager->read(temp, 0, 9);
	cout << temp << endl;

	cout << indexFileManager->length() << endl;

	cout << "IndexFileManager Test Finished." << endl;
}

int main()
{
	printf("DBMS start!\n");
	testDataFileManager();
	testIndexFileManager();
	printf("DBMS shut down!\n");
	system("pause");
    return 0;
}

