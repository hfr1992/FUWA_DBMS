#include "BitMapIterator.h"
#include "../Config.h"
#include "DBFileManager.h"
#include <iostream>
using namespace std;

BitMapIterator::BitMapIterator()
{
	cout << "Can't call the BitMapIterator constructor without any parameter." << endl;
	exit(0);
}

BitMapIterator::BitMapIterator(char * bMap, long size)
{
	bitMap = bMap;
	currentPointer = 0;
	bitMapSize = size;
}

BitMapIterator::~BitMapIterator()
{
}

bool BitMapIterator::hasNext()
{
	return currentPointer < bitMapSize;
}

bool BitMapIterator::next()
{
	bool result = getBitMap(currentPointer);
	currentPointer++;
	return result;
}

bool BitMapIterator::getBitMap(long position)
{
	char which_byte = bitMap[position / 8];
	int which_bit = position % 8;

	char andByte = (char)(power2(7 - which_bit));

	return (which_byte & andByte) >> (7 - which_bit);
}

long BitMapIterator::currentIndex()
{
	return currentPointer - 1;
}
