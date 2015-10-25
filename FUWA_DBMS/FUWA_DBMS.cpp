// FUWA_DBMS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "storage/DBFileManager.h"
#include "storage/BitMapIterator.h"
#include "storage/TableManager.h"
#include <unordered_map>
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
	char * temp2 = (char *)malloc(sizeof(char)*100);

	dBFileManager->append(temp, strlen(temp));
	//dBFileManager->insert("**", 8000, 2);
	//dBFileManager->read(temp2, 6000, 99);
	//temp2[99] = '\0';
	//cout << temp2 << endl;

	cout << dBFileManager->length() << endl;

	cout << "DBFileManager Test Finished." << endl;
}

void testHashMap()
{
	unordered_map<int, int> bhp;
	bhp[1] = 2;
	bhp[2] = 0;
	cout << bhp[3] << endl;
}

void testSetBitMap()
{
	char temp[3] = { 0x08, 0x00, 0x10 };
	BitMapIterator bmi(temp, 24);
	while (bmi.hasNext()) 
	{
		cout << bmi.next() << endl;
	}
	//setBitMap(temp, 9, 1);
	//cout << int(temp[0]) << "_" << int(temp[1]) << "_" << int(temp[2]) << endl;
}

int main()
{
	printf("DBMS start!\n");
	//testDataFileManager();
	//testDBFileManager();
	//testHashMap();
	//cout << sizeof(char) << endl;
	//testSetBitMap();
	//cout << sizeof(int) + sizeof(char) + sizeof(char*) << endl;

	//-------------test-------------
	char * oneTuple = "Hello world!";
	char * secTuple = "Hello, DBMS!!";
	TableManager * tableManager = new TableManager();
	tableManager->insertOneTuple(oneTuple, strlen(oneTuple));
	tableManager->insertOneTuple(secTuple, strlen(secTuple));
	int position = tableManager->insertOneTuple(secTuple, strlen(secTuple));
	cout << position << endl;

	char result[13];
	tableManager->selectOneTuple(result, strlen(secTuple), position);
	for (int i = 0; i < 13; i++)
	{
		printf("%c", result[i]);
	}
	printf("\n");
	tableManager->deleteOneTuple(strlen(secTuple), position);

	char big_string[10000];
	for (int i = 0; i < 10000; i++)
	{
		big_string[i] = 'A';
	}
	tableManager->insertOneTuple(big_string,strlen(big_string));

	tableManager->flush();
	//-------------test-------------

	printf("DBMS shut down!\n");
	system("pause");
    return 0;
}

