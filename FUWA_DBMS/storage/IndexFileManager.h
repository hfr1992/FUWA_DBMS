#include "DataFileManager.h"
#include "../index/BPlusTree.h"
#include "../index/BPlusNode.h"
#include "../Config.h"

class IndexFileManager :public DataFileManager {
public:
	static IndexFileManager * getInstance();
	//void writeIndex(BPlusTree* bPlusTree);
	//void readIndex(BPlusTree* bPlusTree);
	void deleteIndexFile();
	void appendBytes(void* object, long length);
	void read(void * data, long from, long size);
	long length();
private:
	static IndexFileManager * indexFileManager;
	static BPlusTree* bPlusTree;
	IndexFileManager(string fileName);
	~IndexFileManager();
};