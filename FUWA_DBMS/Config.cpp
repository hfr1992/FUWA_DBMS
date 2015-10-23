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