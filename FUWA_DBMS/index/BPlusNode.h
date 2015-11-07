#ifndef BPLUS_NODE
#define BPLUS_NODE
#define NULL 0

enum NODE_TYPE{INTERNAL,LEAF}; //结点类型为：内部结点、叶子节点
enum SIBLING_DIRECTION{LEFT,RIGHT}; //兄弟节点有：左兄弟、右兄弟
typedef long KeyType; //键类型
typedef long DataType; //值类型
const int HALF_ORDER = 3; //树的阶为2*HALF_ORDER=n
const int MIN_KEY = HALF_ORDER - 1; //每个结点的最小键个数
const int MAX_KEY = 2 * HALF_ORDER - 1; //每个结点的最大键个数
const int MIN_CHILD = MIN_KEY + 1; //最小孩子结点个数
const int MAX_CHILD = MAX_KEY + 1; //最大孩子结点个数
const int MIN_LEAF = MIN_KEY; //最小叶子结点的键值对个数
const int MAX_LEAF = MAX_KEY; //最大叶子结点的键值对个数

/*定义基本结点类*/
class FatherNode {
public:
	FatherNode();
	virtual ~FatherNode();

	NODE_TYPE getType() const { return m_Type; }
	void setType(NODE_TYPE type) { m_Type = type; } //结点类型
	int getKeyNum() const { return m_KeyNum; }
	void setKeyNum(int num) { m_KeyNum = num; } //键个数
	KeyType getKeyValue(int v) const { return m_KeyValues[v]; }
	void setKeyValue(int v, KeyType key) { m_KeyValues[v] = key; } //键的值
	int getKeyIndex(KeyType key) const;

	virtual void removeKey(int keyIndex, int childIndex) = 0; //删除键
	virtual void splitNode(FatherNode* parentNode, int childIndex) = 0; //分裂结点
	virtual void mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex) = 0; //合并结点
	virtual void clear() = 0; //清空结点
	virtual void borrowFrom(FatherNode* destNode, FatherNode* parentNode, int keyIndex,SIBLING_DIRECTION d) = 0; //从兄弟结点中借出键
	virtual int getChildIndex(KeyType key, int keyIndex) const = 0; //得到指向孩子结点的指针
	virtual void getBytes(char* bytes) const = 0;

protected:
	NODE_TYPE m_Type; //结点类型
	int m_KeyNum;
	KeyType m_KeyValues[MAX_KEY];
};

/*内部结点*/
class InternalNode :public FatherNode {
public:
	InternalNode();
	virtual ~InternalNode();

	FatherNode* getChild(int v) const { return m_Childs[v]; }
	void setChild(int v, FatherNode* child) { m_Childs[v] = child; } //孩子结点
	void insert(int keyIndex, int childIndex, KeyType key, FatherNode* childNode);

	virtual void splitNode(FatherNode* parentNode, int childIndex);
	virtual void mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(FatherNode* destNode, FatherNode* parentNode, int keyIndex,SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex) const;
	virtual void getBytes(char* bytes) const;

private:
	FatherNode* m_Childs[MAX_CHILD]; //指向孩子结点
};

/*叶子结点*/
class LeafNode :public FatherNode {
public:
	LeafNode();
	virtual ~LeafNode();

	LeafNode* getLeftSibling() const { return m_LeftSibling; }
	void setLeftSibling(LeafNode* node) { m_LeftSibling = node; } //左结点
	LeafNode* getRightSibling() const { return m_RightSibling; }
	void setRightSibling(LeafNode* node) { m_RightSibling = node; } //右结点
	DataType getData(int v) const { return m_Datas[v]; }
	void setData(int v, const DataType &data) { m_Datas[v] = data; } //数据值
	void insert(KeyType key, const DataType &data);

	virtual void splitNode(FatherNode* parentNode, int childIndex);
	virtual void mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(FatherNode* destNode, FatherNode* parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex) const;
	virtual void getBytes(char* bytes) const;

private:
	LeafNode* m_LeftSibling;
	LeafNode* m_RightSibling;
	DataType m_Datas[MAX_LEAF];
};

#endif