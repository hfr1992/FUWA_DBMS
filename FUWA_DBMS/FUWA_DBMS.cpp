// FUWA_DBMS.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "storage/DBFileManager.h"
#include "storage/BitMapIterator.h"
#include "storage/TableManager.h"
#include "Index/BPlusNode.h"
#include "Index/BPlusTree.h"
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
	//insert s1
	tableManager->insertOneTuple(oneTuple, strlen(oneTuple));
	//insert s2
	tableManager->insertOneTuple(secTuple, strlen(secTuple));
	//insert s3
	long position = tableManager->insertOneTuple(secTuple, strlen(secTuple));
	//select s3
	char result[13];
	tableManager->selectOneTuple(result, strlen(secTuple), position);
	for (int i = 0; i < 13; i++)
	{
		printf("%c", result[i]);
	}
	printf("\n");
	//delete s3
	tableManager->deleteOneTuple(strlen(secTuple), position);
	//insert big string s5
	char big_string[20000];
	for (int i = 0; i < 20000; i++)
	{
		big_string[i] = 'A';
	}
	long bs_position = tableManager->insertOneTuple(big_string, 20000);

	//delete s5
	//tableManager->deleteOneTuple(20000, bs_position);

	tableManager->flush();
	//-------------test-------------

	printf("DBMS shut down!\n");

	cout << "The B+ Tree start!" << endl;
	BPlusTree* bPlusTree = new BPlusTree();
	bPlusTree->printTree();
	int key, pos;
	srand((unsigned)time(NULL));
	for (int i = 0; i < 100; i++) {
		//key = rand();
		//pos = rand();
		key = i;
		pos = i ;
		bPlusTree->insert(key, pos);
	}
	bool x = bPlusTree->search(300);
	cout << x << endl;
    //bool z=bPlusTree->remove(60);
	//cout << z << endl;
	bool y = bPlusTree->search(60);
	cout << y << endl;
	//bPlusTree->insert(500, 500);
	//bPlusTree->insert(320, 320);
	//bPlusTree->printTree();
	bPlusTree->printData();



	system("pause");
    return 0;
}

