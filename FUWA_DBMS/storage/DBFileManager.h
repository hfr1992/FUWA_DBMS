#include "DataFileManager.h"
#include "../Config.h"

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
	struct DB_FILE_Header * getDBFileHeader();
	void setDBFileHeader(struct DB_FILE_Header * Header);
private:
	static DBFileManager * dBFileManager;
	struct DB_FILE_Header dB_FILE_Header;
	DBFileManager(string fileName);
	~DBFileManager();
	void getBitMap();
	void createBitMap();
};