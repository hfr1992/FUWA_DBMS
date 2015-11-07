#ifndef BPLUS_NODE
#define BPLUS_NODE
#define NULL 0

enum NODE_TYPE{INTERNAL,LEAF}; //�������Ϊ���ڲ���㡢Ҷ�ӽڵ�
enum SIBLING_DIRECTION{LEFT,RIGHT}; //�ֵܽڵ��У����ֵܡ����ֵ�
typedef long KeyType; //������
typedef long DataType; //ֵ����
const int HALF_ORDER = 3; //���Ľ�Ϊ2*HALF_ORDER=n
const int MIN_KEY = HALF_ORDER - 1; //ÿ��������С������
const int MAX_KEY = 2 * HALF_ORDER - 1; //ÿ��������������
const int MIN_CHILD = MIN_KEY + 1; //��С���ӽ�����
const int MAX_CHILD = MAX_KEY + 1; //����ӽ�����
const int MIN_LEAF = MIN_KEY; //��СҶ�ӽ��ļ�ֵ�Ը���
const int MAX_LEAF = MAX_KEY; //���Ҷ�ӽ��ļ�ֵ�Ը���

/*������������*/
class FatherNode {
public:
	FatherNode();
	virtual ~FatherNode();

	NODE_TYPE getType() const { return m_Type; }
	void setType(NODE_TYPE type) { m_Type = type; } //�������
	int getKeyNum() const { return m_KeyNum; }
	void setKeyNum(int num) { m_KeyNum = num; } //������
	KeyType getKeyValue(int v) const { return m_KeyValues[v]; }
	void setKeyValue(int v, KeyType key) { m_KeyValues[v] = key; } //����ֵ
	int getKeyIndex(KeyType key) const;

	virtual void removeKey(int keyIndex, int childIndex) = 0; //ɾ����
	virtual void splitNode(FatherNode* parentNode, int childIndex) = 0; //���ѽ��
	virtual void mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex) = 0; //�ϲ����
	virtual void clear() = 0; //��ս��
	virtual void borrowFrom(FatherNode* destNode, FatherNode* parentNode, int keyIndex,SIBLING_DIRECTION d) = 0; //���ֵܽ���н����
	virtual int getChildIndex(KeyType key, int keyIndex) const = 0; //�õ�ָ���ӽ���ָ��
	virtual void getBytes(char* bytes) const = 0;

protected:
	NODE_TYPE m_Type; //�������
	int m_KeyNum;
	KeyType m_KeyValues[MAX_KEY];
};

/*�ڲ����*/
class InternalNode :public FatherNode {
public:
	InternalNode();
	virtual ~InternalNode();

	FatherNode* getChild(int v) const { return m_Childs[v]; }
	void setChild(int v, FatherNode* child) { m_Childs[v] = child; } //���ӽ��
	void insert(int keyIndex, int childIndex, KeyType key, FatherNode* childNode);

	virtual void splitNode(FatherNode* parentNode, int childIndex);
	virtual void mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(FatherNode* destNode, FatherNode* parentNode, int keyIndex,SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex) const;
	virtual void getBytes(char* bytes) const;

private:
	FatherNode* m_Childs[MAX_CHILD]; //ָ���ӽ��
};

/*Ҷ�ӽ��*/
class LeafNode :public FatherNode {
public:
	LeafNode();
	virtual ~LeafNode();

	LeafNode* getLeftSibling() const { return m_LeftSibling; }
	void setLeftSibling(LeafNode* node) { m_LeftSibling = node; } //����
	LeafNode* getRightSibling() const { return m_RightSibling; }
	void setRightSibling(LeafNode* node) { m_RightSibling = node; } //�ҽ��
	DataType getData(int v) const { return m_Datas[v]; }
	void setData(int v, const DataType &data) { m_Datas[v] = data; } //����ֵ
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