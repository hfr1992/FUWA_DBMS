#include "DataFileManager.h"

class DictionaryFileManager :public DataFileManager {
public:
	static DictionaryFileManager * getInstance();
private:
	static DictionaryFileManager * dictionaryFileManager;
protected:
	DictionaryFileManager(string fileName);
	~DictionaryFileManager();
};