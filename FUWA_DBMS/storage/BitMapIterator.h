class BitMapIterator
{
public:
	BitMapIterator(char * bMap, long size);
	~BitMapIterator();
	bool hasNext();
	bool next();
private:
	char * bitMap;
	long currentPointer;
	long bitMapSize;
	bool getBitMap(long position);
};