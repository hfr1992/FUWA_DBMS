#include "BufferManager.h"
#include "DBFileManager.h"
#include "BitMapIterator.h"

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
	dBFileManager->read(buffer.bufferUnit[eliminatePointer].data, (pageNumber - 1)*(PAGE_SIZE), PAGE_SIZE);
	buffer.bufferUnit[eliminatePointer].flagSCH = 1;
	buffer.bufferUnit[eliminatePointer].pageNumber = pageNumber;
	bufferHashMap[pageNumber] = eliminatePointer;
	eliminatePointer = (eliminatePointer + 1) % (BUFFER_SIZE);
}

void BufferManager::exportOnePage(long pageNumber)
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	dBFileManager->insert(buffer.bufferUnit[eliminatePointer].data, (pageNumber - 1)*(PAGE_SIZE), PAGE_SIZE);
}

void BufferManager::setFileHeader(long pageNumber, bool flag)
{
	setBitMap(dB_FILE_Header.bitMap, pageNumber - 1, flag);

	DBFileManager * dBFileManager = DBFileManager::getInstance();
	dBFileManager->setDBFileHeader(&dB_FILE_Header);
}

bool BufferManager::isPageWritable(long pageNumber)
{
	char temp[PAGE_SIZE];
	read(temp, pageNumber, 0, PAGE_SIZE);
	int freeSpace = 0;
	for (int i = 0; i < PAGE_SIZE; i++)
	{
		if (temp[i] == '\0')
		{
			freeSpace++;
		}
		else
		{
			freeSpace = 0;
		}
		if (freeSpace >= 5 * sizeof(char))
		{
			return true;
		}
	}
	return false;
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

long BufferManager::findFreeSpace(int spaceSize)
{
	char temp[PAGE_SIZE];
	BitMapIterator bmi(dB_FILE_Header.bitMap, MAX_PAGE_NUM / 8);
	while (bmi.hasNext())
	{
		if (bmi.next() == 0) 
		{
			read(temp, (bmi.currentIndex() + 1), 0, PAGE_SIZE);
			int freeSpace = 0;
			for (int i = 0; i < PAGE_SIZE; i++)
			{
				if (temp[i] == '\0')
				{
					freeSpace++;
				}
				else
				{
					freeSpace = 0;
				}
				if (freeSpace == spaceSize)
				{
					return getPhysicalAddress(bmi.currentIndex() + 1, i - spaceSize + 1);
				}
			}
		}
	}
	return -1;
}

long BufferManager::insertOneTuple(char * tuple, int size)
{
	long insertPosition = findFreeSpace(size);
	write(tuple, getPageNumber(insertPosition), getPagePosition(insertPosition), size);
	if (isPageWritable(getPageNumber(insertPosition)) == false)
	{
		setFileHeader(getPageNumber(insertPosition), 1);
	}
	return insertPosition;
}

void BufferManager::selectOneTuple(char * tuple, int size, long position)
{
	read(tuple, getPageNumber(position), getPagePosition(position), size);
}

void BufferManager::deleteOneTuple(int size, long position)
{
	char * temp = (char *)malloc( size * sizeof(char) );
	for (int i = 0; i < size; i++)
	{
		temp[i] = '\0';
	}
	write(temp, getPageNumber(position), getPagePosition(position), size);

	setFileHeader(getPageNumber(position), 0);

	free(temp);
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

	//Import the header of the DB File (BitMap)
	memcpy(&dB_FILE_Header, dBFileManager->getDBFileHeader(), sizeof(DB_FILE_Header));
}

BufferManager::~BufferManager()
{
}

