// FUWA_DBMS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "storage/DataFileManager.h"
using namespace std;

void fileManagerTest() {
	DataFileManager * dataFileManager = DataFileManager::getInstance();

	dataFileManager->append("Hello, FUWAs!");
	dataFileManager->insert(5, "**");

	char temp[10] = {'\0'};
	dataFileManager->read(temp, 0, 9);
	cout << temp << endl;

	cout << dataFileManager->length() << endl;
}

int main()
{
	printf("DBMS start!\n");
	fileManagerTest();
	printf("DBMS shut down!\n");
	system("pause");
    return 0;
}

