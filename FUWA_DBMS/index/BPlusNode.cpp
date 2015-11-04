#include "BPlusNode.h"

/*基本节点*/
FatherNode::FatherNode() {
	setType(LEAF);
	setKeyNum(0);
}

FatherNode::~FatherNode() {
	setKeyNum(0);
}

/*获得键下标*/
int FatherNode::getKeyIndex(KeyType key) const {
	int left = 0;
	int right = getKeyNum() - 1;
	int current;

	while (left != right) {
		current = (left + right) / 2;
		KeyType currentKey = getKeyValue(current);
		if (key > currentKey) {
			left = current + 1;
		} //折半查找
		else {
			right = current;
		}
	}
	return left;
}

/*内部结点*/
InternalNode::InternalNode() :FatherNode() {
	setType(INTERNAL);
}

InternalNode::~InternalNode() {

}

/*插入内部结点*/
void InternalNode::insert(int keyIndex, int childIndex, KeyType key, FatherNode* childNode) {
	int i;
	for (i = getKeyNum(); i > keyIndex; --i) {
		setChild(i + 1, m_Childs[i]);
		setKeyValue(i, m_KeyValues[i - 1]);
	} //将父节点的childIndex后所有键值对后移一个单位
	if (i == childIndex) {
		setChild(i + 1, m_Childs[i]);
	}
	setChild(childIndex, childNode); //插入新结点
	setKeyValue(keyIndex, key);
	setKeyNum(m_KeyNum + 1);
}

/*分裂内部结点，此种情况发生在原先结点的keyNum=MAX_KEY下*/
void InternalNode::splitNode(FatherNode* parentNode, int childIndex) {
	InternalNode* newNode = new InternalNode(); //分裂后的新结点
	newNode->setKeyNum(MIN_KEY);
	int i;
	for (i = 0; i < MIN_KEY; ++i) {
		newNode->setKeyValue(i, m_KeyValues[i + MIN_CHILD]);
	} //向新结点中拷贝键值
	for (i = 0; i < MIN_CHILD; ++i) {
		newNode->setChild(i, m_Childs[i + MIN_CHILD]);
	} //向新结点中拷贝指针
	setKeyNum(MIN_KEY); //更新原先结点中的键值个数
	((InternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MIN_KEY], newNode); //将新结点插入树中
}

/*合并内部结点，此种情况发生在删除记录后结点的keyNum<MIN_KEY下*/
void InternalNode::mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex) {
	insert(MIN_KEY, MIN_KEY + 1, parentNode->getKeyValue(keyIndex), ((InternalNode*)childNode)->getChild(0));
	int i;
	for (i = 1; i <= childNode->getKeyNum(); ++i) {
		insert(MIN_KEY + i, MIN_KEY + i + 1, childNode->getKeyValue(i - 1), ((InternalNode*)childNode)->getChild(i));
	} //向原先结点中插入相邻结点的键值对
	parentNode->removeKey(keyIndex, keyIndex + 1);
	delete ((InternalNode*)parentNode)->getChild(keyIndex+1); //删除相邻结点
}

/*删除内部结点的键值*/
void InternalNode::removeKey(int keyIndex, int childIndex) {
	for (int i = 0; i < getKeyNum() - keyIndex - 1; ++i) {
		setKeyValue(keyIndex + i, getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}//键值对依次向前移动
	setKeyNum(getKeyNum() - 1);
}

/*清空结点*/
void InternalNode::clear() {
	for (int i = 0; i <= m_KeyNum; ++i) {
		m_Childs[i]->clear();
		delete m_Childs[i];
		m_Childs[i] = NULL;
	}
}

/*从兄弟结点中借出键值对*/
void InternalNode::borrowFrom(FatherNode* siblingNode, FatherNode* parentNode, int keyIndex, SIBLING_DIRECTION d) {
	switch (d) {
	case LEFT: {
		insert(0, 0, parentNode->getKeyValue(keyIndex), ((InternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum());
	} //从左兄弟获得新的键值
		break;
	case RIGHT: {
		insert(getKeyNum(), getKeyNum() + 1, parentNode->getKeyValue(keyIndex), ((InternalNode*)siblingNode)->getChild(0));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		siblingNode->removeKey(0, 0);
	} //从右兄弟获得新的键值
		break;
	default:
		break;
	}
}

/*获得孩子结点的下标*/
int InternalNode::getChildIndex(KeyType key, int keyIndex) const {
	if (key >= getKeyValue(keyIndex)) {
		return keyIndex + 1;
	}
	else {
		return keyIndex;
	}
}

/*叶子节点*/
LeafNode::LeafNode() :FatherNode() {
	setType(LEAF);
	setLeftSibling(NULL);
	setRightSibling(NULL);
}

LeafNode::~LeafNode() {

}

/*插入叶子节点*/
void LeafNode::insert(KeyType key, const DataType &data) {
	int i;
	for (i = m_KeyNum; i >= 1 && m_KeyValues[i - 1] > key; --i) {
		setKeyValue(i, m_KeyValues[i - 1]);
		setData(i, m_Datas[i - 1]);
	}
	setKeyValue(i, key);
	setData(i, data);
	setKeyNum(m_KeyNum + 1);
}

void LeafNode::splitNode(FatherNode* parentNode, int childIndex) {
	LeafNode* newNode = new LeafNode();
	setKeyNum(MIN_LEAF);
	newNode->setKeyNum(MIN_LEAF + 1);
	newNode->setRightSibling(getRightSibling());
	setRightSibling(newNode);
	newNode->setLeftSibling(this);
	int i;
	for (i = 0; i < MIN_LEAF + 1; ++i) {
		newNode->setKeyValue(i, m_KeyValues[i + MIN_LEAF]);
	}
	for (i = 0; i < MIN_LEAF + 1; ++i) {
		newNode->setData(i, m_Datas[i + MIN_LEAF]);
	}
	((InternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MIN_LEAF], newNode);
}

void LeafNode::mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex) {
	for (int i = 0; i < childNode->getKeyNum(); ++i) {
		insert(childNode->getKeyValue(i), ((LeafNode*)childNode)->getData(i));
	}
	setRightSibling(((LeafNode*)childNode)->getRightSibling());
	parentNode->removeKey(keyIndex, keyIndex + 1);
}

void LeafNode::removeKey(int keyIndex, int childIndex) {
	for (int i = keyIndex; i < getKeyNum() - 1; ++i) {
		setKeyValue(i, getKeyValue(i + 1));
		setData(i, getData(i + 1));
	}
	setKeyNum(getKeyNum() - 1);
}

void LeafNode::clear() {

}

void LeafNode::borrowFrom(FatherNode* siblingNode, FatherNode* parentNode, int keyIndex, SIBLING_DIRECTION d) {
	switch (d)
	{
	case LEFT: {
		insert(siblingNode->getKeyValue(siblingNode->getKeyNum() - 1), ((LeafNode*)siblingNode)->getData(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum() - 1);
		parentNode->setKeyValue(keyIndex, getKeyValue(0));
	}
		break;
	case RIGHT: {
		insert(siblingNode->getKeyValue(0), ((LeafNode*)siblingNode)->getData(0));
		siblingNode->removeKey(0, 0);
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
	}
		break;
	default:
		break;
	}
}

int LeafNode::getChildIndex(KeyType key, int keyIndex) const {
	return keyIndex;
}