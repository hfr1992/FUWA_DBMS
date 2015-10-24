#include "../Config.h"
#include <unordered_map>
using namespace std;

struct BufferUnit
{
	int pageNumber = -1;//from 1 to n
	int flagSCH = 0;
	char data[SIZE_PER_PAGE];
};

struct Buffer
{
	struct BufferUnit bufferUnit[BUFFER_SIZE];
};

class BufferManager
{
public:
	static BufferManager * getInstance();
	void read(char * data,long pageNumber, long from, long size);
	void write(char * data, long pageNumber, long from, long size);
private:
	static BufferManager * bufferManager;
	long eliminatePointer;
	struct Buffer buffer;
	unordered_map<int, int> bufferHashMap;
	BufferManager();
	~BufferManager();
	long findPage(long pageNumber);
	long exchangePages(long pageNumber);
	void importOnePage(long pageNumber);
	void exportOnePage(long pageNumber);
};