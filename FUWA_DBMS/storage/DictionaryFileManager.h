#include "DataFileManager.h"

class DictionaryFileManager :public DataFileManager {
public:
	static DictionaryFileManager * getInstance();
private:
	static DictionaryFileManager * dictionaryFileManager;
	DictionaryFileManager(string fileName);
	~DictionaryFileManager();
};