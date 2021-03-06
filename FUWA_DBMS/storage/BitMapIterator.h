class BitMapIterator
{
public:
	BitMapIterator(char * bMap, long size);
	~BitMapIterator();
	bool hasNext();
	bool next();
	long currentIndex();
private:
	BitMapIterator();
	char * bitMap;
	long currentPointer;
	long bitMapSize;
	long currentPageNumber;
	bool getBitMap(long position);
};