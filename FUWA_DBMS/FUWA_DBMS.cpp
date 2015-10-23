// FUWA_DBMS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "storage/DBFileManager.h"
using namespace std;

//void testDataFileManager() 
//{
//	cout << "Testing DataFileManager..." << endl;
//
//	DataFileManager * dataFileManager = DataFileManager::getInstance();
//
//	dataFileManager->append("Hello, Data!");
//	dataFileManager->insert(5, "**");
//
//	char temp[10] = {'\0'};
//	dataFileManager->read(temp, 0, 9);
//	cout << temp << endl;
//
//	cout << dataFileManager->length() << endl;
//
//	cout << "DataFileManager Test Finished." << endl;
//}

void testDBFileManager()
{
	cout << "Testing DBFileManager..." << endl;

	DBFileManager * dBFileManager = DBFileManager::getInstance();

	const char * temp = "Hello, world!";

	dBFileManager->append(temp, strlen(temp));
	dBFileManager->insert(5, "**", 2);

	cout << dBFileManager->length() << endl;

	cout << "DBFileManager Test Finished." << endl;
}

int main()
{
	printf("DBMS start!\n");
	//testDataFileManager();
	testDBFileManager();
	//cout << sizeof(char) << endl;
	printf("DBMS shut down!\n");
	system("pause");
    return 0;
}

