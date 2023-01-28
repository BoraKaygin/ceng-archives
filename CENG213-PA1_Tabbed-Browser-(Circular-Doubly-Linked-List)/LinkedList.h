#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

using namespace std;

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);
    ~LinkedList();

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

    int getSize() const;
    bool isEmpty() const;
    bool containsNode(Node<T> *node) const;
    int getIndex(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void append(const T &data);
    void prepend(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAtIndex(const T &data, int index);
    void moveToIndex(int currentIndex, int newIndex);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeNodeAtIndex(int index);
    void removeAllNodes();

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    int size;
};

template<class T>
LinkedList<T>::LinkedList()
{
    /* TODO */
    head = NULL;
    size = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj)
{
    /* TODO */
    if (obj.isEmpty())
    {
        head = NULL;
        size = 0;
        return;
    }
    size = obj.size;
    head = new Node<T>(obj.head->data);
    Node<T> *currentNew = head, *currentRight = obj.head;
    for (int i = 0; i < size-1; ++i)
    {
        currentNew->next = new Node<T>(currentRight->next->data);
        currentNew->next->prev = currentNew;
        currentNew = currentNew->next;
        currentRight = currentRight->next;
    }
    head->prev = currentNew;
    currentNew->next = head;
}

template<class T>
LinkedList<T>::~LinkedList()
{
    /* TODO */
    if (not head) return;
    if (head->next == head)
    {
        delete head;
        head = NULL;
        return;
    }
    Node<T> *current = head->next;
    while (current != head)
    {
        current = current->next;
        delete current->prev;
    }
    delete current;
    head = NULL;
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs) // Heavily copied code, check first in case of error
{
    /* TODO */
    if (this != &rhs)
    {
        if (not head) goto EMPTY_ASSIGN;
        if (head->next == head)
        {
            delete head;
        }
        else
        {
            Node<T> *current = head->next;
            while (current != head) {
                current = current->next;
                delete current->prev;
            }
            delete current;
        }
        EMPTY_ASSIGN: if (rhs.isEmpty())
        {
            head = NULL;
            size = 0;
            return *this;
        }
        size = rhs.size;
        head = new Node<T>(rhs.head->data);
        Node<T> *currentNew = head, *currentRight = rhs.head;
        for (int i = 0; i < size-1; ++i)
        {
            currentNew->next = new Node<T>(currentRight->next->data);
            currentNew->next->prev = currentNew;
            currentNew = currentNew->next;
            currentRight = currentRight->next;
        }
        head->prev = currentNew;
        currentNew->next = head;
    }
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    /* TODO */
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const
{
    /* TODO */
    if (head) return false;
    else return true;
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    /* TODO */
    if (not head || not node) return false;
    Node<T> *current = head->next;
    while (current!=head)
    {
        if (current==node) return true;
        current = current->next;
    }
    if (current==node) return true;
    else return false;
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    /* TODO */
    if (not head || not node) return false;
    Node<T> *current = head;
    for(int index = 0;index < size;index++)
    {
        if (current == node) return index;
        current = current->next;
    }
    return -1;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    /* TODO */
    return head;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    /* TODO */
    if (not head) return NULL;
    return head->prev;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const
{
    /* TODO */
    if (not head) return NULL;
    Node<T> *current = head;
    for(int index = 0;index < size;index++)
    {
        if (current->data == data) return current;
        current = current->next;
    }
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    /* TODO */
    if (index < 0 || index >= size) return NULL;
    Node<T> *current = head;
    for (int i = 0; i < index; ++i)
    {
        current = current->next;
    }
    return current;
}

template<class T>
void LinkedList<T>::append(const T &data)
{
    /* TODO */
    if (not head)
    {
        head = new Node<T>(data);
        head->next = head;
        head->prev = head;
        size++;
        return;
    }
    head->prev->next = new Node<T>(data,head->prev,head);
    head->prev = head->prev->next;
    size++;
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    /* TODO */
    this->append(data);
    head = head->prev;
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    /* TODO */
    if (not head || not node) return;
    if (head == node)
    {
        head->next->prev = new Node<T>(data,head,head->next);
        head->next = head->next->prev;
        size++;
        return;
    }
    Node<T> *current = head->next;
    while (current != head)
    {
        if (current == node)
        {
            current->next->prev = new Node<T>(data,current,current->next);
            current->next = current->next->prev;
            size++;
            return;
        }
        current = current->next;
    }
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    /* TODO */
    if (index < 0 || index > size) return;
    if (not head && not index)
    {
        head = new Node<T>(data,head,head);
        size++;
        return;
    }
    Node<T> *current = head;
    for (int i = 0; i < index; ++i)
    {
        current = current->next;
    }
    current->prev = new Node<T>(data,current->prev,current);
    current->prev->prev->next = current->prev;
    size++;
}

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    /* TODO */
    if (newIndex >= size) newIndex = size-1;
    if (currentIndex < 0 || newIndex < 0 || currentIndex >= size || currentIndex == newIndex) return;
    Node<T> *movingNode = getNodeAtIndex(currentIndex), *positionNode = getNodeAtIndex(newIndex);
    if (currentIndex == 0) head = head->next;
    movingNode->next->prev = movingNode->prev;
    movingNode->prev->next = movingNode->next;
    if (currentIndex < newIndex)
    {
        movingNode->prev = positionNode;
        movingNode->next = positionNode->next;
    }
    else if (currentIndex > newIndex)
    {
        movingNode->prev = positionNode->prev;
        movingNode->next = positionNode;
    }
    movingNode->prev->next = movingNode;
    movingNode->next->prev = movingNode;
    if (newIndex == 0) head = head->prev;
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    /* TODO */
    if (not node || not head) return;
    if (node == head)
    {
        head->next->prev = head->prev;
        head->prev->next = head->next;
        Node<T> * tmp = head;
        head = head->next;
        delete tmp;
        size--;
        return;
    }
    Node<T> *current = head->next;
    while (current != head)
    {
        if (current == node)
        {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            size--;
            return;
        }
        current = current->next;
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{
    /* TODO */
    if (not head) return;
    while (head->data == data)
    {
        if (head->next == head)
        {
            delete head;
            head = NULL;
            size--;
            return;
        }
        head->prev->next = head->next;
        head->next->prev = head->prev;
        Node<T> *tmp = head;
        head = head->next;
        delete tmp;
        size--;
    }
    Node <T> *current = head->next;
    while (current != head)
    {
        if (current->data == data)
        {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            Node <T> *tmp = current;
            current = current->next;
            delete tmp;
            size--;
            continue;
        }
        current = current->next;
    }
}

template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    /* TODO */
    if (not head || index < 0 || index >= size) return;
    if (index == 0)
    {
        head->prev->next = head->next;
        head->next->prev = head->prev;
        Node<T> *tmp = head;
        head = head->next;
        delete tmp;
        size --;
        if (not size) head = NULL;
        return;
    }
    Node<T> *current = head;
    for (int i = 0; i < index; ++i)
    {
        current = current->next;
    }
    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    size--;
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    /* TODO */
    if (not head) return;
    if (head->next == head)
    {
        delete head;
        head = NULL;
        size = 0;
        return;
    }
    Node<T> *current = head->next;
    while(current != head)
    {
        current = current->next;
        delete current->prev;
    }
    delete head;
    head = NULL;
    size = 0;
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    do {
        std::cout << *node << std::endl;
        node = node->next;
    }
    while (node != this->getFirstNode());
}

#endif //LINKEDLIST_H
