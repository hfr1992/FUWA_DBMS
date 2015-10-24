struct Table_1
{
	int col_int;
	char col_char;
	char * col_string;
};

class TableManager
{
public:
	TableManager();
	~TableManager();
private:
	void createTestData(struct Table_1 * tuples);
	void deleteTestData(struct Table_1 * tuples);
};