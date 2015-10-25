#include "../Config.h"
#include <unordered_map>
using namespace std;

struct BufferUnit
{
	int pageNumber = -1;//from 1 to n
	int flagSCH = 0;
	char data[PAGE_SIZE];
};

struct Buffer
{
	struct BufferUnit bufferUnit[BUFFER_SIZE];
};

class BufferManager
{
public:
	static BufferManager * getInstance();
	long findFreeSpace(int spaceSize);
	long insertOneTuple(char * tuple, int size);
	void selectOneTuple(char * tuple, int size, long position);
	void deleteOneTuple(int size, long position);
private:
	static BufferManager * bufferManager;
	long eliminatePointer;
	struct Buffer buffer;
	unordered_map<int, int> bufferHashMap;
	struct DB_FILE_Header dB_FILE_Header;
	BufferManager();
	~BufferManager();
	void read(char * data, long pageNumber, long from, long size);
	void write(char * data, long pageNumber, long from, long size);
	long findPage(long pageNumber);
	long exchangePages(long pageNumber);
	void importOnePage(long pageNumber);
	void exportOnePage(long pageNumber);
	void setFileHeader(long position, bool flag);
	bool isPageWritable(long pageNumber);
};