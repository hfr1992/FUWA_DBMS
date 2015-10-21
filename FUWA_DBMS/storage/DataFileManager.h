#include<iostream>
#include<io.h>
using namespace std;

class DataFileManager {
public:
	//Get this class
	static DataFileManager * getInstance();
	//Read from the data file
	void read(char * data, long from, long size);
	//Insert into the file
	void insert(long from, const char * data);
	//Append to the file
	void append(const char * data);
	//Get the length of the file
	long length();
private:
	//File pointer
	FILE * fp;
	//Name of the file
	static string dataFileName;
	//Point to this class
	static DataFileManager * dataFileManager;
	//Constructor
	DataFileManager();
	//Destructor
	virtual ~DataFileManager();
	//Set the file pointer
	void setPointer(long offset, int fromwhere);
};