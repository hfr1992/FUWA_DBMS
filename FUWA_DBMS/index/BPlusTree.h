#ifndef BPLUS_TREE_HEADER
#define BPLUS_TREE_HEADER
#define DIVISION 10 //分隔符的个数，用于打印树

#include "BPlusNode.h"
#include <vector>
using namespace std;

enum COMPARE_OPERATOR {LT,LE,EQ,RE,RT,BETWEEN}; //<、<=、=、>=、>、<>
const int INVALID_INDEX = -1;
struct SelectResult {
	int keyIndex;
	LeafNode* targetNode;
};

class BPlusTree {
public:
	BPlusTree();
	~BPlusTree();

	bool insert(KeyType key, const DataType &data);
	bool remove(KeyType key);
	//bool update(KeyType oldKey, KeyType newKey);
	//vector<DataType> select(KeyType compareKey, int compareOperator);
	//vector<DataType> select(KeyType smallKey, KeyType largeKey);
	bool search(KeyType key);
	void clear();
	void printTree() const;
	void printData() const;
private:
	FatherNode* m_Root; //根结点
	LeafNode* m_DataHead;
	KeyType m_MaxKey; //B+树的最大键

	void recursive_insert(FatherNode* parentNode, KeyType key, const DataType &data);
	void recursive_remove(FatherNode* parentNode, KeyType key);
	bool recursive_search(FatherNode* pNode, KeyType key) const;
	void changeKey(FatherNode* pNode, KeyType oldKey, KeyType newKey);
	void printInNode(FatherNode* pNode, int count) const;
	void search(KeyType key, SelectResult &result);
	void recursive_search(FatherNode* pNode, KeyType key, SelectResult &result);
	void remove(KeyType key, DataType &data);
	void recursive_remove(FatherNode* parentNode, KeyType key, DataType &data);
};

#endif