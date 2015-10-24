#include "DataFileManager.h"
#include "../Config.h"

struct DB_FILE_Header
{
	int bitMap[MAX_PAGE_NUM/8];
};

class DBFileManager :public DataFileManager {
public:
	static DBFileManager * getInstance();
	//Read from the data file
	void read(char * data, long from, long size);
	//Insert into the file
	void insert(const char * data, long from, int length);
	//Append to the file
	void append(const char * data, int length);
	//Get the length of the file
	long length();
private:
	static DBFileManager * dBFileManager;
	DBFileManager(string fileName);
	~DBFileManager();
};