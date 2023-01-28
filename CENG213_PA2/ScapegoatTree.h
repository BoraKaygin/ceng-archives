#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"

enum TraversalMethod {preorder, inorder, postorder};

template<class T>
class ScapegoatTree {
public: // DO NOT CHANGE THIS PART.
    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp=inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;
    void copyHelper(Node<T> *&newNode, Node<T> * const &copiedNode);
    int getHeight(Node<T> *nodePtr) const;
    int getSize(Node<T> *nodePtr) const;
    void removeAllNodes(Node<T> *&nodePtr);
    bool insert(const T &element,Node<T> *&nodePtr,bool &condition);
    void balance(Node<T> *&nodePtr);
    void inorderArray(Node<T> *nodePtr, int &index, T *array) const;
    void buildFromArray(Node<T> *&nodePtr, T *nodeArray, int firstIndex, int lastIndex);
    bool remove(Node<T> *&nodePtr,const T &element);
    const T &get(Node<T> *nodePtr,const T &element) const;
    void getCeiling(Node<T> *nodePtr,const T &element, Node<T> *&ptrToCeil) const;
    void getFloor(Node<T> *nodePtr,const T &element, Node<T> *&ptrToFloor) const;
    const T &getMin(Node<T> *nodePtr) const;
    const T &getMax(Node<T> *nodePtr) const;
    void getNext(Node<T> *nodePtr,const T &element, Node<T> *&ptrToNext) const;

private: // DO NOT CHANGE THIS PART.
    Node<T> *root;

    int upperBound;
};

#endif //TREE_H

template<class T>
ScapegoatTree<T>::ScapegoatTree() {
    root = NULL;
    upperBound = 0;
}

template<class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj) {
    upperBound = obj.upperBound;
    copyHelper(root,obj.root);
}

template<class T>
void ScapegoatTree<T>::copyHelper(Node<T> *&newNode, Node<T> * const &copiedNode)
{
    if (not copiedNode)
    {
        newNode = NULL;
        return;
    }
    newNode = new Node<T>;
    newNode->element = copiedNode->element;
    copyHelper(newNode->left,copiedNode->left);
    copyHelper(newNode->right,copiedNode->right);
}

template<class T>
ScapegoatTree<T>::~ScapegoatTree() {
    removeAllNodes(root);
}

template<class T>
bool ScapegoatTree<T>::isEmpty() const {
    return (not root);
}

template<class T>
int ScapegoatTree<T>::getHeight() const {
    return getHeight(root);
}

template<class T>
int ScapegoatTree<T>::getHeight(Node<T> *nodePtr) const
{
    if (not nodePtr)
    {
        return -1;
    }
    return 1 + std::max(getHeight(nodePtr->left),getHeight(nodePtr->right));
}

template<class T>
int ScapegoatTree<T>::getSize() const {
    return getSize(root);
}

template<class T>
int ScapegoatTree<T>::getSize(Node<T> *nodePtr) const
{
    if (not nodePtr)
        return 0;
    return 1 + getSize(nodePtr->left) + getSize(nodePtr->right);
}

template<class T>
bool ScapegoatTree<T>::insert(const T &element) {
    bool condition = true;
    return insert(element,root,condition);
}

template<class T>
bool ScapegoatTree<T>::insert(const T &element,Node<T> *&nodePtr,bool &condition)
{
    if (not nodePtr)
    {
        nodePtr = new Node<T>;
        nodePtr->element = element;
        upperBound++;
        if (pow(1.5,getHeight()) > upperBound)
        {
            condition = false;
        }
        return true;
    }
    else if (element < nodePtr->element)
    {
        int result = insert(element,nodePtr->left,condition);
        if (not condition)
        {
            if (3 * getSize(nodePtr->left) > 2 * getSize(nodePtr))
            {
                balance(nodePtr);
                condition = true;
            }
        }
        return result;
    }
    else if (element > nodePtr->element)
    {
        int result = insert(element,nodePtr->right,condition);
        if (not condition)
        {
            if (3*getSize(nodePtr->right) > 2 * getSize(nodePtr))
            {
                balance(nodePtr);
                condition = true;
            }
        }
        return result;
    }
    return false;
}

template<class T>
void ScapegoatTree<T>::balance(Node<T> *&nodePtr)
{
    int size = getSize(nodePtr);
    T *nodes = new T[size];
    int i = 0;
    inorderArray(nodePtr,i,nodes);
    removeAllNodes(nodePtr);
    buildFromArray(nodePtr,nodes,0,size-1);
    delete [] nodes;
}

template<class T>
void ScapegoatTree<T>::inorderArray(Node<T> *nodePtr, int &index, T *array) const
{
    if (not nodePtr)
        return;
    inorderArray(nodePtr->left,index,array);
    array[index++] = nodePtr->element;
    inorderArray(nodePtr->right,index,array);
}

template<class T>
void ScapegoatTree<T>::buildFromArray(Node<T> *&nodePtr, T *nodeArray, int firstIndex, int lastIndex)
{
    if (firstIndex == lastIndex)
    {
        nodePtr = new Node<T>;
        nodePtr->element = nodeArray[firstIndex];
        return;
    }
    else if (firstIndex+1 == lastIndex)
    {
        nodePtr = new Node<T>;
        nodePtr->element = nodeArray[firstIndex];
        nodePtr->right = new Node<T>;
        nodePtr->right->element = nodeArray[lastIndex];
    }
    else
    {
        nodePtr = new Node<T>;
        nodePtr->element = nodeArray[(lastIndex+firstIndex)/2];
        buildFromArray(nodePtr->left,nodeArray,firstIndex,(lastIndex+firstIndex)/2 - 1);
        buildFromArray(nodePtr->right,nodeArray,(lastIndex+firstIndex)/2 + 1,lastIndex);
    }
}

template<class T>
bool ScapegoatTree<T>::remove(const T &element) {
    bool result = remove(root,element);
    if (not result)
    {
        return false;
    }
    int size = getSize(root);
    if (size == 0)
    {
        upperBound = 0;
        return result;
    }
    if (upperBound > 2*size || size > upperBound)
    {
        balance();
        upperBound = size;
    }
    return result;
}

template<class T>
bool ScapegoatTree<T>::remove(Node<T> *&nodePtr, const T &element)
{
    if (nodePtr)
    {
        if (nodePtr->element == element)
        {
            if (nodePtr->left && nodePtr->right)
            {
                nodePtr->element = getMax(nodePtr->left);
                remove(nodePtr->left,nodePtr->element);
            }
            else if (nodePtr->left)
            {
                Node<T>* tmp = nodePtr;
                nodePtr = nodePtr->left;
                delete tmp;
            }
            else if (nodePtr->right)
            {
                Node<T>* tmp = nodePtr;
                nodePtr = nodePtr->right;
                delete tmp;
            }
            else
            {
                delete nodePtr;
                nodePtr = NULL;
            }
            return true;
        }
        else if (element < nodePtr->element) return remove(nodePtr->left,element);
        else return remove(nodePtr->right,element);
    }
    else return false;
}

template<class T>
void ScapegoatTree<T>::removeAllNodes() {
    removeAllNodes(root);
    upperBound = 0;
}

template<class T>
void ScapegoatTree<T>::removeAllNodes(Node<T> *&nodePtr)
{
    if (not nodePtr)
    {
        return;
    }
    removeAllNodes(nodePtr->left);
    removeAllNodes(nodePtr->right);
    delete nodePtr;
    nodePtr = NULL;
}

template<class T>
const T &ScapegoatTree<T>::get(const T &element) const {
    return get(root,element);
}

template<class T>
const T &ScapegoatTree<T>::get(Node<T> *nodePtr, const T &element) const
{
    if (!nodePtr) throw NoSuchItemException();
    else
    {
        if (nodePtr->element == element) return nodePtr->element;
        else if (element < nodePtr->element) return get(nodePtr->left,element);
        else return get(nodePtr->right,element);
    }
}

template<class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const {
    if (tp == preorder) {
        if (isEmpty())
        {
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }
        std::cout << "BST_preorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == inorder) {
        if (isEmpty()) { // check if the tree is empty?
            std::cout << "BST_inorder{}" << std::endl; //the tree is empty
            return;
        }
        std::cout << "BST_inorder{" << std::endl;// the tree is not empty. recursively output the tree.
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == postorder) {
        if (isEmpty())
        {
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }
        std::cout << "BST_postorder{" << std::endl;
        print(root, tp);
        std::cout << std::endl << "}" << std::endl;
    }
}

template<class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const {
    if (tp == preorder) {
        if (node == NULL)
            return;
        else
        {
            std::cout << "\t" << node->element;
            if (node->left)
            {
                std::cout << "," << std::endl;
            }
            print(node->left, preorder);
            if (node->right)
            {
                std::cout << "," << std::endl;
            }
            print(node->right, preorder);
        }
    } else if (tp == inorder) {
        if (node == NULL) // check if the node is NULL?
            return;
        print(node->left, inorder); // first, output left subtree and comma (if needed).
        if (node->left) {
            std::cout << "," << std::endl;
        }
        std::cout << "\t" << node->element; // then, output the node.
        if (node->right) { // finally, output comma (if needed) and the right subtree.
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    } else if (tp == postorder) {
        if (node == NULL)
        {
            return;
        }
        print(node->left, postorder);
        if (node->left)
        {
            std::cout << "," << std::endl;
        }
        print(node->right,postorder);
        if (node->right)
        {
            std::cout << "," << std::endl;
        }
        std::cout << "\t" << node->element;
    }
}

template<class T>
void ScapegoatTree<T>::printPretty() const {
    if (isEmpty()) { // check if the tree is empty?
        std::cout << "BST_pretty{}" << std::endl; // the tree is empty.
        return;
    }
    std::cout << "BST_pretty{" << std::endl; // the tree is not empty. recursively output the tree.
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;
}

template<class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const {
    if (node == NULL) // check if the node is NULL?
        return;
    std::cout << "\t"; // output the indentation and the node.
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;
    printPretty(node->left, indentLevel + 1, true); // first, output left subtree with one more indentation level.
    printPretty(node->right, indentLevel + 1, false); // then, output right subtree with one more indentation level.
}

template<class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs) {
    if (this == &rhs) return *this;
    removeAllNodes();
    upperBound = rhs.upperBound;
    copyHelper(root,rhs.root);
    return *this;
}

template<class T>
void ScapegoatTree<T>::balance() {
    balance(root);
}

template<class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const {
    Node<T> *ptrToCeil = NULL;
    getCeiling(root,element,ptrToCeil);
    if (ptrToCeil) return ptrToCeil->element;
    throw NoSuchItemException();
}

template<class T>
void ScapegoatTree<T>::getCeiling(Node<T> *nodePtr, const T &element,Node<T> *&ptrToCeil) const
{
    if (nodePtr)
    {
        if (nodePtr->element == element)
        {
            ptrToCeil = nodePtr;
            return;
        }
        else if (element < nodePtr->element)
        {
            ptrToCeil = nodePtr;
            getCeiling(nodePtr->left,element,ptrToCeil);
        }
        else
        {
            getCeiling(nodePtr->right,element,ptrToCeil);
        }
    }
}

template<class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const {
    Node<T> *ptrToFloor = NULL;
    getFloor(root,element,ptrToFloor);
    if (ptrToFloor) return ptrToFloor->element;
    throw NoSuchItemException();
}

template<class T>
void ScapegoatTree<T>::getFloor(Node<T> *nodePtr, const T &element, Node<T> *&ptrToFloor) const
{
    if (nodePtr)
    {
        if (nodePtr->element == element)
        {
            ptrToFloor = nodePtr;
            return;
        }
        else if (element < nodePtr->element)
        {
            getFloor(nodePtr->left,element,ptrToFloor);
            return;
        }
        else
        {
            ptrToFloor = nodePtr;
            getFloor(nodePtr->right,element,ptrToFloor);
            return;
        }
    }
}

template<class T>
const T &ScapegoatTree<T>::getMin() const {
    if (root)
    {
        return getMin(root);
    }
    throw NoSuchItemException();    
}

template<class T>
const T &ScapegoatTree<T>::getMin(Node<T> *nodePtr) const
{
    if (nodePtr->left) return getMin(nodePtr->left);
    return nodePtr->element;
}

template<class T>
const T &ScapegoatTree<T>::getMax() const {
    if (root)
    {
        return getMax(root);
    }
    throw NoSuchItemException();
}

template<class T>
const T &ScapegoatTree<T>::getMax(Node<T> *nodePtr) const
{
    if (nodePtr->right) return getMax(nodePtr->right);
    return nodePtr->element;
}

template<class T>
const T &ScapegoatTree<T>::getNext(const T &element) const {
    Node<T> *ptrToNext = NULL;
    getNext(root,element,ptrToNext);
    if (ptrToNext) return ptrToNext->element;
    throw NoSuchItemException();
}

template<class T>
void ScapegoatTree<T>::getNext(Node<T> *nodePtr, const T &element, Node<T> *&ptrToNext) const
{
    if (nodePtr)
    {
        if (nodePtr->element == element || element > nodePtr->element)
        {
            getNext(nodePtr->right,element,ptrToNext);
        }
        else if (element < nodePtr->element)
        {
            ptrToNext = nodePtr;
            getNext(nodePtr->left,element,ptrToNext);
        }
    }
}
