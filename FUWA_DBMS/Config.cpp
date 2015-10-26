#include "Config.h"
#include <iostream>
using namespace std;

void intToBytes(int intNumber, char * result)
{
	result[0] = (char)((0xff000000 & intNumber) >> 24);
	result[1] = (char)((0x00ff0000 & intNumber) >> 16);
	result[2] = (char)((0x0000ff00 & intNumber) >> 8);
	result[3] = (char)((0x000000ff & intNumber));
}

int power2(int times) 
{
	int temp = 1;
	for (int i = 0; i < times; i++)
	{
		temp = temp * 2;
	}
	return temp;
}

void setBitMap(char * bitMap, long position, bool flag)
{
	char which_byte = bitMap[position / 8];
	int which_bit = position % 8;

	int andByte = 0;
	for (int i = 0; i < 8; i++)
	{
		if (i == which_bit) 
		{
			continue;
		}
		else 
		{
			andByte += power2(7-i);
		}
	}

	if (flag == 1)
	{
		//char temp = (which_byte & ((char)andByte)) + ((char)(0x01) << (7 - which_bit));
		//memcpy(&(bitMap[position / 8]), &temp, 1);
		bitMap[position / 8] = (which_byte & ((char)andByte)) + ((char)(0x01) << (7 - which_bit));
	}
	else
	{
		//char temp = (which_byte & ((char)andByte)) + ((char)(0x00) << (7 - which_bit));
		//memcpy(&(bitMap[position / 8]), &temp, 1);
		bitMap[position / 8] = (which_byte & ((char)andByte)) + ((char)(0x00) << (7 - which_bit));
	}

	cout << "BitMap modified byte: " << (int)bitMap[position / 8] << endl;
}

long getPhysicalAddress(int pageNumber, int pagePosition) 
{
	return (MAX_PAGE_NUM / 8) + (pageNumber - 1) * PAGE_SIZE + pagePosition;
}

int getPageNumber(long physicalAddress)
{
	return (((physicalAddress - (MAX_PAGE_NUM / 8)) / (PAGE_SIZE)) + 1);
}

int getPagePosition(long physicalAddress)
{
	return ((physicalAddress - (MAX_PAGE_NUM / 8)) % (PAGE_SIZE));
}

int getFreeSpaceInCurrentPage(long physicalAddress) 
{
	int currentStart = (getPageNumber(physicalAddress) - 1)*PAGE_SIZE + (MAX_PAGE_NUM / 8);
	return PAGE_SIZE - (physicalAddress - currentStart);
}