#include "BPlusTree.h"
#include <iostream>
#include <algorithm>
using namespace std;

BPlusTree::BPlusTree() {
	m_Root = NULL;
	m_DataHead = NULL;
}

BPlusTree::~BPlusTree() {
	clear();
}

bool BPlusTree::insert(KeyType key, const DataType &data) {
	if (search(key)) {
		return false;
	}
	if (m_Root == NULL) {
		m_Root = new LeafNode();
		m_DataHead = (LeafNode*)m_Root;
		m_MaxKey = key;
	}
	if (m_Root->getKeyNum() >= MAX_KEY) {
		InternalNode* newNode = new InternalNode();
		newNode->setChild(0, m_Root);
		m_Root->splitNode(newNode, 0);
		m_Root = newNode;
	}
	if (key > m_MaxKey) {
		m_MaxKey = key;
	}
	recursive_insert(m_Root, key, data);
	return true;
}

bool BPlusTree::search(KeyType key) {
	return recursive_search(m_Root, key);
}

bool BPlusTree::recursive_search(FatherNode* pNode, KeyType key) const {
	if (pNode == NULL) {
		return false;
	}
	else {
		int keyIndex = pNode->getKeyIndex(key);
		int childIndex = pNode->getChildIndex(key, keyIndex);
		if (keyIndex < pNode->getKeyNum() && key == pNode->getKeyValue(keyIndex)) {
			return true;
		}
		else {
			if (pNode->getType() == LEAF) {
				return false;
			}
			else {
				return recursive_search(((InternalNode*)pNode)->getChild(childIndex), key);
			}
		}
	}
}

void BPlusTree::recursive_insert(FatherNode* parentNode, KeyType key, const DataType &data) {
	if (parentNode->getType() == LEAF) {
		((LeafNode*)parentNode)->insert(key, data);
	}
	else {
		int keyIndex = parentNode->getKeyIndex(key);
		int childIndex = parentNode->getChildIndex(key, keyIndex);
		FatherNode* childNode = ((InternalNode*)parentNode)->getChild(childIndex);
		if (childNode->getKeyNum() >= MAX_LEAF) {
			childNode->splitNode(parentNode, childIndex);
			if (parentNode->getKeyValue(childIndex) <= key) {
				childNode = ((InternalNode*)parentNode)->getChild(childIndex+1);
			}
		}
		recursive_insert(childNode, key, data);
	}
}

bool BPlusTree::remove(KeyType key) {
	if (!search(key)) {
		return false;
	}
	if (m_Root->getKeyNum() == 1) {
		if (m_Root->getType() == LEAF) {
			clear();
			return true;
		}
		else {
			FatherNode *pChild1 = ((InternalNode*)m_Root)->getChild(0);
			FatherNode *pChild2 = ((InternalNode*)m_Root)->getChild(1);
			if (pChild1->getKeyNum() == MIN_KEY && pChild2->getKeyNum() == MIN_KEY) {
				pChild1->mergeNode(m_Root, pChild2, 0);
				delete m_Root;
				m_Root = pChild1;
			}
		}
	}
	recursive_remove(m_Root, key);
	return true;
}

void BPlusTree::recursive_remove(FatherNode* parentNode, KeyType key) {
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex);
	if (parentNode->getType() == LEAF) {
		if (key == m_MaxKey && keyIndex > 0) {
			m_MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		parentNode->removeKey(keyIndex, childIndex);
		if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead) {
			changeKey(m_Root, key, parentNode->getKeyValue(0));
		}
	}
	else {
		FatherNode* pChildNode = ((InternalNode*)parentNode)->getChild(childIndex);
		if (pChildNode->getKeyNum() == MIN_KEY) {
			FatherNode* pLeft = childIndex > 0 ? ((InternalNode*)parentNode)->getChild(childIndex - 1) : NULL;
			FatherNode* pRight = childIndex < parentNode->getKeyNum() ? ((InternalNode*)parentNode)->getChild(childIndex + 1) : NULL;
			if (pLeft && pLeft->getKeyNum()>MIN_KEY) {
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight && pRight->getKeyNum() > MIN_KEY) {
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			else if (pLeft) {
				pLeft->mergeNode(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight) {
				pChildNode->mergeNode(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, key);
	}
}

void BPlusTree::changeKey(FatherNode *pNode, KeyType oldKey, KeyType newKey) {
	if (pNode != NULL && pNode->getType() != LEAF) {
		int keyIndex = pNode->getKeyIndex(oldKey);
		if (keyIndex < pNode->getKeyNum() && oldKey == pNode->getKeyValue(keyIndex)) {
			pNode->setKeyValue(keyIndex, newKey);
		}
		else {
			changeKey(((InternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
		}
	}
}

/*bool BPlusTree::update(KeyType oldKey, KeyType newKey) {
	if (search(newKey)) {
		return false;
	}
	else {
		DataType data;
		remove(oldKey, data);
		if (data == INVALID_INDEX) {
			return false;
		}
		else {
			return insert(newKey, data);
		}
	}
}*/

void BPlusTree::remove(KeyType key, DataType &data) {
	if (!search(key)) {
		data = INVALID_INDEX;
		return;
	}
	if (m_Root->getKeyNum() == 1) {
		if (m_Root->getType() == LEAF) {
			data = ((LeafNode*)m_Root)->getData(0);
			clear();
			return;
		}
		else {
			FatherNode* pChild1 = ((InternalNode*)m_Root)->getChild(0);
			FatherNode* pChild2 = ((InternalNode*)m_Root)->getChild(1);
			if (pChild1->getKeyNum() == MIN_KEY && pChild2->getKeyNum() == MIN_KEY) {
				pChild1->mergeNode(m_Root, pChild2, 0);
				delete m_Root;
				m_Root = pChild1;
			}
		}
	}
	recursive_remove(m_Root, key, data);
}

void BPlusTree::recursive_remove(FatherNode* parentNode, KeyType key, DataType &data) {
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex);
	if (parentNode->getType() == LEAF) {
		if (key == m_MaxKey && keyIndex > 0) {
			m_MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		data = ((LeafNode*)parentNode)->getData(keyIndex);
		parentNode->removeKey(keyIndex, childIndex);
		if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead) {
			changeKey(m_Root, key, parentNode->getKeyValue(0));
		}
	}
	else {
		FatherNode* pChildNode = ((InternalNode*)parentNode)->getChild(childIndex);
		if (pChildNode->getKeyNum() == MIN_KEY) {
			FatherNode* pLeft = childIndex > 0 ? ((InternalNode*)parentNode)->getChild(childIndex - 1) : NULL;
			FatherNode* pRight = childIndex < parentNode->getKeyNum() ? ((InternalNode*)parentNode)->getChild(childIndex + 1) : NULL;
			if (pLeft && pLeft->getKeyNum()>MIN_KEY) {
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight && pRight->getKeyNum() > MIN_KEY) {
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			else if (pLeft) {
				pLeft->mergeNode(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight) {
				pChildNode->mergeNode(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, key, data);
	}
}

/*vector<DataType> BPlusTree::select(KeyType compareKey, int compareOperator) {
	vector<DataType> results;
	if (m_Root != NULL) {
		if (compareKey > m_MaxKey) {
			if (compareOperator == LE || compareOperator == LT) {
				for (LeafNode* itr = m_DataHead; itr != NULL; itr = itr->getRightSibling()) {
					for (int i = 0; i < itr->getKeyNum(); ++i) {
						results.push_back(itr->getData(i));
					}
				}
			}
		}
		else if (compareKey < m_DataHead->getKeyValue(0)) {
			if (compareOperator == RE || compareOperator == RT) {
				for (LeafNode* itr = m_DataHead; itr != NULL; itr = itr->getRightSibling()) {
					for (int i = 0; i < itr->getKeyNum(); ++i) {
						results.push_back(itr->getData(i));
					}
				}
			}
		}
		else {
			SelectResult result;
			search(compareKey, result);
			switch (compareOperator) {
			case LT:
			case LE: {
				LeafNode* itr = m_DataHead;
				int i;
				while (itr!=result.targetNode) {
					for (i = 0; i < itr->getKeyNum(); ++i) {
						results.push_back(itr->getData(i));
					}
					itr = itr->getRightSibling();
				}
				for (i = 0; i < result.keyIndex; ++i) {
					results.push_back(itr->getData(i));
				}
				if (itr->getKeyValue(i) < compareKey || (compareOperator == LE && compareKey == itr->getKeyValue(i))) {
					results.push_back(itr->getData(i));
				}
			}
				break;
			case EQ: {
				if (result.targetNode->getKeyValue(result.keyIndex) == compareKey) {
					results.push_back(result.targetNode->getData(result.keyIndex));
				}
			}
				 break;
			case RE:
			case RT: {
				LeafNode* itr = result.targetNode;
				if (compareKey < itr->getKeyValue(result.keyIndex) || (compareOperator == RE && compareKey == itr->getKeyValue(result.keyIndex))) {
					results.push_back(itr->getData(result.keyIndex));
				}
				int i;
				for (i = result.keyIndex + 1; i < itr->getKeyNum(); ++i) {
					results.push_back(itr->getData(i));
				}
				itr = itr->getRightSibling();
				while (itr!=NULL) {
					for (i = 0; i < itr->getKeyNum(); ++i) {
						results.push_back(itr->getData(i));
					}
					itr = itr->getRightSibling();
				}
			}
				 break;
			default:
				break;
			}
		}
	}
	sort<vector<DataType>::iterator>(results.begin(), results.end());
	return results;
}*/

/*vector<DataType> BPlusTree::select(KeyType smallKey, KeyType largeKey) {
	vector<DataType> results;
	if (smallKey < largeKey) {
		SelectResult start, end;
		search(smallKey, start);
		search(largeKey, end);
		LeafNode* itr = start.targetNode;
		int i = start.keyIndex;
		if (itr->getKeyValue(i) < smallKey) {
			++i;
		}
		if (end.targetNode->getKeyValue(end.keyIndex) > largeKey) {
			--end.keyIndex;
		}
		while (itr != end.targetNode) {
			for (; i < itr->getKeyNum(); ++i) {
				results.push_back(itr->getData(i));
			}
			itr = itr->getRightSibling();
			i = 0;
		}
		for (; i <= end.keyIndex; ++i) {
			results.push_back(itr->getData(i));
		}
	}
	sort<vector<DataType>::iterator>(results.begin(), results.end());
	return results;
}*/

void BPlusTree::search(KeyType key, SelectResult &result) {
	recursive_search(m_Root, key, result);
}

void BPlusTree::recursive_search(FatherNode* pNode, KeyType key, SelectResult &result) {
	int keyIndex = pNode->getKeyIndex(key);
	int childIndex = pNode->getChildIndex(key, keyIndex);
	if (pNode->getType() == LEAF) {
		result.keyIndex = keyIndex;
		result.targetNode = (LeafNode*)pNode;
		return;
	}
	else {
		return recursive_search(((InternalNode*)pNode)->getChild(childIndex), key, result);
	}
}

void BPlusTree::clear() {
	if (m_Root != NULL) {
		m_Root->clear();
		delete m_Root;
		m_Root = NULL;
		m_DataHead = NULL;
	}
}

void BPlusTree::printTree() const {
	printInNode(m_Root, DIVISION);
}

void BPlusTree::printInNode(FatherNode *pNode, int division) const {
	if (pNode != NULL) {
		int i, j;
		for (i = 0; i < pNode->getKeyNum(); ++i) {
			if (pNode->getType() != LEAF) {
				printInNode(((InternalNode*)pNode)->getChild(i), division - 2);
			}
			for (j = division; j >= 0; --j) {
				cout << "--";
			}
			cout << pNode->getKeyValue(i) << endl;
		}
		if (pNode->getType()!=LEAF) {
			printInNode(((InternalNode*)pNode)->getChild(i), division - 2);
		}
	}
	else {
		cout << "The B+ Tree is empty!" << endl;
	}
}

void BPlusTree::printData() const {
	LeafNode* itr = m_DataHead;
	while (itr != NULL) {
		for (int i = 0; i < itr->getKeyNum(); ++i) {
			cout << itr->getData(i) << " ";
		}
		cout <<endl;
		itr = itr->getRightSibling();
	}
	//cout << endl;
}