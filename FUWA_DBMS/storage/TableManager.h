//struct Table_1
//{
//	int col_int;
//	char col_char;
//	char * col_string;
//};

class TableManager
{
public:
	TableManager();
	~TableManager();
	long insertOneTuple(char * tuple, int size);
	void selectOneTuple(char * tuple, int size, long position);
	void deleteOneTuple(int size, long position);
	void flush();
//private:
//	void createTestData(struct Table_1 * tuples);
//	void deleteTestData(struct Table_1 * tuples);
};