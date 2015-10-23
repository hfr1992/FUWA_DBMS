#include "DataFileManager.h"

class IndexFileManager :public DataFileManager {
public:
	static IndexFileManager * getInstance();
private:
	static IndexFileManager * indexFileManager;
	IndexFileManager(string fileName);
	~IndexFileManager();
};