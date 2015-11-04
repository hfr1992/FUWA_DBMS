#include "BPlusNode.h"

/*�����ڵ�*/
FatherNode::FatherNode() {
	setType(LEAF);
	setKeyNum(0);
}

FatherNode::~FatherNode() {
	setKeyNum(0);
}

/*��ü��±�*/
int FatherNode::getKeyIndex(KeyType key) const {
	int left = 0;
	int right = getKeyNum() - 1;
	int current;

	while (left != right) {
		current = (left + right) / 2;
		KeyType currentKey = getKeyValue(current);
		if (key > currentKey) {
			left = current + 1;
		} //�۰����
		else {
			right = current;
		}
	}
	return left;
}

/*�ڲ����*/
InternalNode::InternalNode() :FatherNode() {
	setType(INTERNAL);
}

InternalNode::~InternalNode() {

}

/*�����ڲ����*/
void InternalNode::insert(int keyIndex, int childIndex, KeyType key, FatherNode* childNode) {
	int i;
	for (i = getKeyNum(); i > keyIndex; --i) {
		setChild(i + 1, m_Childs[i]);
		setKeyValue(i, m_KeyValues[i - 1]);
	} //�����ڵ��childIndex�����м�ֵ�Ժ���һ����λ
	if (i == childIndex) {
		setChild(i + 1, m_Childs[i]);
	}
	setChild(childIndex, childNode); //�����½��
	setKeyValue(keyIndex, key);
	setKeyNum(m_KeyNum + 1);
}

/*�����ڲ���㣬�������������ԭ�Ƚ���keyNum=MAX_KEY��*/
void InternalNode::splitNode(FatherNode* parentNode, int childIndex) {
	InternalNode* newNode = new InternalNode(); //���Ѻ���½��
	newNode->setKeyNum(MIN_KEY);
	int i;
	for (i = 0; i < MIN_KEY; ++i) {
		newNode->setKeyValue(i, m_KeyValues[i + MIN_CHILD]);
	} //���½���п�����ֵ
	for (i = 0; i < MIN_CHILD; ++i) {
		newNode->setChild(i, m_Childs[i + MIN_CHILD]);
	} //���½���п���ָ��
	setKeyNum(MIN_KEY); //����ԭ�Ƚ���еļ�ֵ����
	((InternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MIN_KEY], newNode); //���½���������
}

/*�ϲ��ڲ���㣬�������������ɾ����¼�����keyNum<MIN_KEY��*/
void InternalNode::mergeNode(FatherNode* parentNode, FatherNode* childNode, int keyIndex) {
	insert(MIN_KEY, MIN_KEY + 1, parentNode->getKeyValue(keyIndex), ((InternalNode*)childNode)->getChild(0));
	int i;
	for (i = 1; i <= childNode->getKeyNum(); ++i) {
		insert(MIN_KEY + i, MIN_KEY + i + 1, childNode->getKeyValue(i - 1), ((InternalNode*)childNode)->getChild(i));
	} //��ԭ�Ƚ���в������ڽ��ļ�ֵ��
	parentNode->removeKey(keyIndex, keyIndex + 1);
	delete ((InternalNode*)parentNode)->getChild(keyIndex+1); //ɾ�����ڽ��
}

/*ɾ���ڲ����ļ�ֵ*/
void InternalNode::removeKey(int keyIndex, int childIndex) {
	for (int i = 0; i < getKeyNum() - keyIndex - 1; ++i) {
		setKeyValue(keyIndex + i, getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}//��ֵ��������ǰ�ƶ�
	setKeyNum(getKeyNum() - 1);
}

/*��ս��*/
void InternalNode::clear() {
	for (int i = 0; i <= m_KeyNum; ++i) {
		m_Childs[i]->clear();
		delete m_Childs[i];
		m_Childs[i] = NULL;
	}
}

/*���ֵܽ���н����ֵ��*/
void InternalNode::borrowFrom(FatherNode* siblingNode, FatherNode* parentNode, int keyIndex, SIBLING_DIRECTION d) {
	switch (d) {
	case LEFT: {
		insert(0, 0, parentNode->getKeyValue(keyIndex), ((InternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum());
	} //�����ֵܻ���µļ�ֵ
		break;
	case RIGHT: {
		insert(getKeyNum(), getKeyNum() + 1, parentNode->getKeyValue(keyIndex), ((InternalNode*)siblingNode)->getChild(0));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		siblingNode->removeKey(0, 0);
	} //�����ֵܻ���µļ�ֵ
		break;
	default:
		break;
	}
}

/*��ú��ӽ����±�*/
int InternalNode::getChildIndex(KeyType key, int keyIndex) const {
	if (key >= getKeyValue(keyIndex)) {
		return keyIndex + 1;
	}
	else {
		return keyIndex;
	}
}

/*Ҷ�ӽڵ�*/
LeafNode::LeafNode() :FatherNode() {
	setType(LEAF);
	setLeftSibling(NULL);
	setRightSibling(NULL);
}

LeafNode::~LeafNode() {

}

/*����Ҷ�ӽڵ�*/
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