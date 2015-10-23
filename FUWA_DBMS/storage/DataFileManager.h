#include<iostream>
#include<io.h>
using namespace std;

class DataFileManager {
private:
	//File pointer
	FILE * fp;
	//Name of the file
	static string dataFileName;
	//Set the file pointer
	void setPointer(long offset, int fromwhere);
	//Point to this class
	static DataFileManager * dataFileManager;
protected:
	//Constructor
	DataFileManager(string fileName);
	//Destructor
	virtual ~DataFileManager();
	//Get this class
	static DataFileManager * getInstance();
	//Read from the data file
	void read(char * data, long from, long size);
	//Insert into the file
	void insert(long from, const char * data, int length);
	//Append to the file
	void append(const char * data, int length);
	//Get the length of the file
	long length();
};