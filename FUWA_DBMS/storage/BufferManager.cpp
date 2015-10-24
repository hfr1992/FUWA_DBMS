#include "BufferManager.h"
#include "DBFileManager.h"

BufferManager * BufferManager::bufferManager = 0;

BufferManager * BufferManager::getInstance()
{
	if (bufferManager == 0) 
	{
		bufferManager = new BufferManager();
	}
	return bufferManager;
}

//Use the SCH exchange method
long BufferManager::exchangePages(long pageNumber) 
{
	while (buffer.bufferUnit[eliminatePointer].flagSCH>0) 
	{
		buffer.bufferUnit[eliminatePointer].flagSCH--;
		eliminatePointer = (eliminatePointer + 1) % (BUFFER_SIZE);
	}

	exportOnePage(pageNumber);
	importOnePage(pageNumber);

	//return the last position before the eliminate pointer
	return ((eliminatePointer + BUFFER_SIZE) - 1) % (BUFFER_SIZE);
}

void BufferManager::importOnePage(long pageNumber)
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	dBFileManager->read(buffer.bufferUnit[eliminatePointer].data, (pageNumber - 1)*(SIZE_PER_PAGE), SIZE_PER_PAGE);
	buffer.bufferUnit[eliminatePointer].flagSCH = 1;
	buffer.bufferUnit[eliminatePointer].pageNumber = pageNumber;
	bufferHashMap[pageNumber] = eliminatePointer;
	eliminatePointer = (eliminatePointer + 1) % (BUFFER_SIZE);
}

void BufferManager::exportOnePage(long pageNumber)
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	dBFileManager->insert(buffer.bufferUnit[eliminatePointer].data, (pageNumber - 1)*(SIZE_PER_PAGE), SIZE_PER_PAGE);
}

long BufferManager::findPage(long pageNumber)
{
	long pagePosition = bufferHashMap[pageNumber];
	if (pagePosition != 0) 
	{
		return pagePosition;
	}
	else
	{
		return exchangePages(pageNumber);
	}
}

void BufferManager::read(char * data, long pageNumber, long from, long size)
{
	struct BufferUnit bUnit = buffer.bufferUnit[findPage(pageNumber)];
	memcpy(data, bUnit.data + from, size);
	bUnit.flagSCH = 1;
}

void BufferManager::write(char * data, long pageNumber, long from, long size)
{
	struct BufferUnit bUnit = buffer.bufferUnit[findPage(pageNumber)];
	memcpy(bUnit.data + from, data, size);
	bUnit.flagSCH = 1;
}

BufferManager::BufferManager() 
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	eliminatePointer = 0;

	long DBFileLength = dBFileManager->length();
	long times = DBFileLength / BUFFER_SIZE;

	for (long i = 0; i < times; i++)
	{
		importOnePage(i+1);
	}
}

BufferManager::~BufferManager()
{
}

