
#ifndef MY_MAP_H
#define MY_MAP_H

#include <iostream>
using namespace std;

template<typename KeyType, typename ValueType>  // Syntax for template class forward declaration
class BinarySearchTree;

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);
	const ValueType* find(const KeyType& key) const;
    bool empty() const;
	ValueType* find(const KeyType& key)
	{
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;
private:
    BinarySearchTree<KeyType,ValueType> m_tree;
};

template<typename KeyType, typename ValueType>
MyMap<KeyType,ValueType>::MyMap()
{
    // intentionally blank
}

template<typename KeyType, typename ValueType>
MyMap<KeyType,ValueType>::~MyMap()
{
    m_tree.clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType,ValueType>::clear()
{
    // Add later - to clear map so the container size is 0
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType,ValueType>::size() const
{
    return m_tree.size();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType,ValueType>::associate(const KeyType& key, const ValueType& value)
{
    m_tree.addNode(key,value);
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType,ValueType>::find(const KeyType& key) const
{
    return m_tree.find(key);
}

template<typename KeyType, typename ValueType>
bool MyMap<KeyType,ValueType>::empty() const
{
    if (m_tree.empty())
        return true;
    else
        return false;
}

template<typename KeyType, typename ValueType>
struct BinarySearchTreeNode
{
    KeyType keyValue;
    ValueType otherValue;
    BinarySearchTreeNode* leftPtr;
    BinarySearchTreeNode* rightPtr;
    bool operator < (const BinarySearchTreeNode<KeyType, ValueType>& other) const;
    bool operator > (const BinarySearchTreeNode<KeyType, ValueType>& other) const;
    bool operator == (const BinarySearchTreeNode<KeyType, ValueType>& other) const;
};

template<typename KeyType, typename ValueType>
class BinarySearchTree
{
public:
    BinarySearchTree();
    ~BinarySearchTree();
    void addNode(const KeyType& key, const ValueType& val);
    const ValueType* find(const KeyType& key) const;
    int size() const;
    bool empty() const;
    void clear();
private:
    int sizeHelper(const BinarySearchTreeNode<KeyType,ValueType>* subtreePtr) const;
    const ValueType* findHelper(const BinarySearchTreeNode<KeyType,ValueType>* subtreePtr, const KeyType& key) const;
    void clearTree(BinarySearchTreeNode<KeyType,ValueType>* subtreePtr);
    BinarySearchTreeNode<KeyType,ValueType>* rootPtr;
};

template<typename KeyType, typename ValueType>
bool BinarySearchTree<KeyType, ValueType>::empty() const
{
    if (rootPtr == nullptr)
        return true;
    else
        return false;
}

template<typename KeyType, typename ValueType>
const ValueType* BinarySearchTree<KeyType, ValueType>::findHelper(const BinarySearchTreeNode<KeyType,ValueType>* subtreePtr, const KeyType& key) const
{
    if (subtreePtr == nullptr)
    {
        return nullptr;
    }
    
    if (subtreePtr->keyValue == key)
    {
        const ValueType* ptr = &(subtreePtr->otherValue);
        return ptr;

    }
    if (key < subtreePtr->keyValue)
    {
        const ValueType* leftRes = findHelper(subtreePtr->leftPtr, key);
        if (leftRes != nullptr)
            return leftRes;
    }
    else
    {
       const ValueType* rightRes = findHelper(subtreePtr->rightPtr, key);
        if (rightRes != nullptr)
            return rightRes;
    }
    return nullptr;
}

template<typename KeyType, typename ValueType>
const ValueType* BinarySearchTree<KeyType, ValueType>::find(const KeyType& key) const
{
    return findHelper(rootPtr, key);
}

template<typename KeyType, typename ValueType>
int BinarySearchTree<KeyType, ValueType>::size() const
{
    return sizeHelper(rootPtr);
}


template<typename KeyType, typename ValueType>
BinarySearchTree<KeyType, ValueType>::BinarySearchTree()
: rootPtr(nullptr)
{
}

template<typename KeyType, typename ValueType>
BinarySearchTree<KeyType, ValueType>::~BinarySearchTree()
{
    clearTree(rootPtr);
}

template<typename KeyType, typename ValueType>
void BinarySearchTree<KeyType, ValueType>::clear()
{
    clearTree(rootPtr);
    rootPtr = nullptr;
}

template<typename KeyType, typename ValueType>
void BinarySearchTree<KeyType, ValueType>::clearTree(BinarySearchTreeNode<KeyType,ValueType>* subtreePtr)
{
    if (subtreePtr == nullptr)     // Is this okay?
        return;
    clearTree(subtreePtr->leftPtr);
    clearTree(subtreePtr->rightPtr);
    delete subtreePtr;
}

template<typename KeyType, typename ValueType>
void BinarySearchTree<KeyType, ValueType>::addNode(const KeyType& key, const ValueType& val)
{
    if (rootPtr == nullptr)
    {
        rootPtr = new BinarySearchTreeNode<KeyType, ValueType>;
        rootPtr->keyValue = key;
        rootPtr->otherValue = val;
        rootPtr->leftPtr = nullptr;
        rootPtr->rightPtr = nullptr;
        return;
    }

    const ValueType* ptrToExisting = find(key);
    if (ptrToExisting != nullptr)
    {
        ValueType* nonConstPtr = const_cast<ValueType*>(ptrToExisting);
        // Is casting away the constness the best option to overwrite the item in the map?
        *nonConstPtr = val;
    }
    else
    {
        BinarySearchTreeNode<KeyType, ValueType>* newNode = new BinarySearchTreeNode<KeyType, ValueType>;
        newNode->keyValue = key;
        newNode->otherValue = val;
        newNode->leftPtr = nullptr;
        newNode->rightPtr = nullptr;
        
        BinarySearchTreeNode<KeyType, ValueType>* subtreePtr = rootPtr;
        
        while (subtreePtr != nullptr)
        {
            if ( (*newNode < *subtreePtr) && (subtreePtr->leftPtr != nullptr))
            {
                subtreePtr = subtreePtr->leftPtr;
            }
            else if( (*newNode < *subtreePtr) && (subtreePtr->leftPtr == nullptr))
            {
                subtreePtr->leftPtr = newNode;
                subtreePtr = nullptr;   // To end the search
            }
            else if ( (*newNode > *subtreePtr) && (subtreePtr->rightPtr != nullptr))
            {
                subtreePtr = subtreePtr->rightPtr;
            }
            else
            {
                subtreePtr->rightPtr = newNode;
                subtreePtr = nullptr;    // To end the search
            }
        }
    }
}



template<typename KeyType, typename ValueType>
bool BinarySearchTreeNode<KeyType, ValueType>::operator < (const BinarySearchTreeNode<KeyType, ValueType>& other) const
{
    return (keyValue < other.keyValue);
}

template<typename KeyType, typename ValueType>
bool BinarySearchTreeNode<KeyType, ValueType>::operator == (const BinarySearchTreeNode<KeyType, ValueType>& other) const
{
    return (keyValue == other.keyValue);
}

template<typename KeyType, typename ValueType>
bool BinarySearchTreeNode<KeyType, ValueType>::operator > (const BinarySearchTreeNode<KeyType, ValueType>& other) const
{
    return (keyValue > other.keyValue);
}

template<typename KeyType, typename ValueType>
int BinarySearchTree<KeyType, ValueType>::sizeHelper(const BinarySearchTreeNode<KeyType,ValueType>* subtreePtr) const
{
    if (subtreePtr == nullptr)
        return 0;
    int leftCount, rightCount = 0;
    leftCount = sizeHelper(subtreePtr->leftPtr);
    rightCount = sizeHelper(subtreePtr->rightPtr);
    
    return 1 + leftCount + rightCount;
}

#endif
