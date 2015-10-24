#include "Config.h"
#include <iostream>
using namespace std;

void intToBytes(const char * data, char * result)
{
	unsigned int strLenInt = strlen(data);
	result[0] = (char)((0xff000000 & strLenInt) >> 24);
	result[1] = (char)((0x00ff0000 & strLenInt) >> 16);
	result[2] = (char)((0x0000ff00 & strLenInt) >> 8);
	result[3] = (char)((0x000000ff & strLenInt));
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

	bitMap[position / 8] = (which_byte & ((char)andByte)) + ((char)(0x01) << (7 - which_bit));
}