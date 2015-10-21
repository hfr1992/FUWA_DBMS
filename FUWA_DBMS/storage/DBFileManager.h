#include "DataFileManager.h"

class DBFileManager :public DataFileManager {
public:
	static DBFileManager * getInstance();
private:
	static DBFileManager * dBFileManager;
protected:
	DBFileManager(string fileName);
	~DBFileManager();
};