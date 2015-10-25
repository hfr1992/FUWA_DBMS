#include "BufferManager.h"
#include "DBFileManager.h"
#include "BitMapIterator.h"

BufferManager * BufferManager::bufferManager = 0;

BufferManager * BufferManager::getInstance()
{
	if (bufferManager == 0) 
	{
		bufferManager = new BufferManager();
		cout << "Create BufferManager." << endl;
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

	if (buffer.bufferUnit[eliminatePointer].pageNumber != -1)
	{
		cout << "Export page: " << eliminatePointer << "." << endl;
		exportOnePage(buffer.bufferUnit[eliminatePointer].pageNumber);
	}
	cout << "Import page: " << pageNumber << "." << endl;
	importOnePage(pageNumber);

	eliminatePointer = (eliminatePointer + 1) % (BUFFER_SIZE);

	//return the last position before the eliminate pointer
	return ((eliminatePointer + BUFFER_SIZE) - 1) % (BUFFER_SIZE);
}

void BufferManager::importOnePage(long pageNumber)
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	if (dBFileManager->length() >= (pageNumber)*(PAGE_SIZE)+(MAX_PAGE_NUM / 8))
	{
		dBFileManager->read(buffer.bufferUnit[eliminatePointer].data, (pageNumber - 1)*(PAGE_SIZE)+(MAX_PAGE_NUM / 8), PAGE_SIZE);
	}
	else
	{
		for (int i = 0; i < PAGE_SIZE; i++)
		{
			buffer.bufferUnit[eliminatePointer].data[i] = '\0';
		}
	}
	
	buffer.bufferUnit[eliminatePointer].flagSCH = 1;
	buffer.bufferUnit[eliminatePointer].pageNumber = pageNumber;
	bufferHashMap[pageNumber] = eliminatePointer + 1;
}

void BufferManager::exportOnePage(long pageNumber)
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	dBFileManager->insert(buffer.bufferUnit[eliminatePointer].data, (pageNumber - 1)*(PAGE_SIZE)+(MAX_PAGE_NUM / 8), PAGE_SIZE);
	bufferHashMap[pageNumber] = 0;
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
	long pagePosition = bufferHashMap[pageNumber] - 1;
	if (pagePosition >= 0) 
	{
		cout << "The page " << pageNumber << " required is in the buffer, pagePosition: " << pagePosition << "." << endl;
		return pagePosition;
	}
	else
	{
		cout << "Exchange the page " << pageNumber << " from disk to buffer." << endl;
		return exchangePages(pageNumber);
	}
}

void BufferManager::read(char * data, long pageNumber, long from, long size)
{
	cout << "Read from page:" << pageNumber << " position:" << from << endl;
	struct BufferUnit * bUnit = &(buffer.bufferUnit[findPage(pageNumber)]);
	memcpy(data, bUnit->data + from, size);
	bUnit->flagSCH = 1;
}

void BufferManager::write(char * data, long pageNumber, long from, long size)
{
	cout << "Write to page:" << pageNumber << " position:" << from << endl;
	struct BufferUnit * bUnit = &(buffer.bufferUnit[findPage(pageNumber)]);
	memcpy(bUnit->data + from, data, size);
	bUnit->flagSCH = 1;
}

long BufferManager::findFreeSpace(int spaceSize)
{
	char temp[PAGE_SIZE];
	BitMapIterator bmi(dB_FILE_Header.bitMap, MAX_PAGE_NUM / 8);
	int freeSpace = 0;
	while (bmi.hasNext())
	{
		if (bmi.next() == 0) 
		{
			read(temp, (bmi.currentIndex() + 1), 0, PAGE_SIZE);
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
	DBFileManager * dBFileManager = DBFileManager::getInstance();

	cout << "----------------Finding free space----------------" << endl;
	long insertPosition = findFreeSpace(size);
	if (insertPosition == -1)
	{
		long fileLength = dBFileManager->length();
		if (fileLength < MAX_PAGE_NUM * PAGE_SIZE)
		{
			insertPosition = fileLength;
		}
		else
		{
			cout << "Out of max size of DB File." << endl;
			exit(0);
		}
	}
	cout << "Find the enough free space from position:'" << insertPosition << "'." << endl;
	cout << "----------------Finding free space----------------" << endl;
	cout << "----------------Inserting----------------" << endl;
	if (BUFFER_SIZE - getPagePosition(insertPosition) > size)
	{
		//Just one segment
		cout << "Just one segment. PageNumber: " << getPageNumber(insertPosition) << " PagePosition: " << getPagePosition(insertPosition) << " Size: " << size <<" BitMap: "<< isPageWritable(getPageNumber(insertPosition)) <<"."<< endl;
		write(tuple, getPageNumber(insertPosition), getPagePosition(insertPosition), size);
		if (isPageWritable(getPageNumber(insertPosition)) == false) setFileHeader(getPageNumber(insertPosition), 1);
	}
	else
	{
		//More than one segment
		//First segment
		write(tuple, getPageNumber(insertPosition), getPagePosition(insertPosition), BUFFER_SIZE - getPagePosition(insertPosition));
		if (isPageWritable(getPageNumber(insertPosition)) == false) setFileHeader(getPageNumber(insertPosition), 1);
		size -= (BUFFER_SIZE - getPagePosition(insertPosition));
		tuple += (BUFFER_SIZE - getPagePosition(insertPosition));
		insertPosition += (BUFFER_SIZE - getPagePosition(insertPosition));

		//Second to last second segments
		while (size > BUFFER_SIZE) 
		{
			write(tuple, getPageNumber(insertPosition), getPagePosition(insertPosition), BUFFER_SIZE);
			setFileHeader(getPageNumber(insertPosition), 1);
			size -= BUFFER_SIZE;
			tuple += BUFFER_SIZE;
			insertPosition += BUFFER_SIZE;
		}

		//last segment
		write(tuple, getPageNumber(insertPosition), getPagePosition(insertPosition), size);
		if (isPageWritable(getPageNumber(insertPosition)) == false) setFileHeader(getPageNumber(insertPosition), 1);
	}
	cout << "----------------Inserting----------------" << endl;
	return insertPosition;
}

void BufferManager::selectOneTuple(char * tuple, int size, long position)
{
	read(tuple, getPageNumber(position), getPagePosition(position), size);
}

void BufferManager::deleteOneTuple(int size, long position)
{
	if (BUFFER_SIZE - getPagePosition(position) > size)
	{
		char * temp = (char *)malloc(size * sizeof(char));
		for (int i = 0; i < size; i++) temp[i] = '\0';
		write(temp, getPageNumber(position), getPagePosition(position), size);
		setFileHeader(getPageNumber(position), 0);
		free(temp);
	}
	else
	{
		//More than one segment
		//First segment
		char * temp = (char *)malloc((BUFFER_SIZE - getPagePosition(position)) * sizeof(char));
		for (int i = 0; i < (BUFFER_SIZE - getPagePosition(position)); i++) temp[i] = '\0';
		write(temp, getPageNumber(position), getPagePosition(position), (BUFFER_SIZE - getPagePosition(position)));
		setFileHeader(getPageNumber(position), 0);
		size -= (BUFFER_SIZE - getPagePosition(position));
		position += (BUFFER_SIZE - getPagePosition(position));
		free(temp);

		//Second to last second segments
		temp = (char *)malloc(BUFFER_SIZE * sizeof(char));
		for (int i = 0; i < BUFFER_SIZE; i++) temp[i] = '\0';
		while (size > BUFFER_SIZE)
		{
			write(temp, getPageNumber(position), getPagePosition(position), BUFFER_SIZE);
			setFileHeader(getPageNumber(position), 0);
			size -= BUFFER_SIZE;
			position += BUFFER_SIZE;
		}
		free(temp);

		//last segment
		temp = (char *)malloc(size * sizeof(char));
		for (int i = 0; i < size; i++) temp[i] = '\0';
		write(temp, getPageNumber(position), getPagePosition(position), size);
		setFileHeader(getPageNumber(position), 0);
		free(temp);

	}

}

void BufferManager::flush()
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	cout << "----------------Flushing----------------" << endl;
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		if (bufferHashMap[buffer.bufferUnit[i].pageNumber] > 0)
		{
			dBFileManager->insert(buffer.bufferUnit[i].data, (buffer.bufferUnit[i].pageNumber - 1)*(PAGE_SIZE)+(MAX_PAGE_NUM / 8), PAGE_SIZE);
		}
	}
	cout << "----------------Flushing----------------" << endl;
}

BufferManager::BufferManager() 
{
	DBFileManager * dBFileManager = DBFileManager::getInstance();
	eliminatePointer = 0;

	long DBFileLength = dBFileManager->length();
	long times = ( DBFileLength - (MAX_PAGE_NUM / 8) * sizeof(char) ) / BUFFER_SIZE;

	cout << "The length of the DB File is:'" << DBFileLength << "' bytes, split into:'" << times << "' pages." << endl;

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

