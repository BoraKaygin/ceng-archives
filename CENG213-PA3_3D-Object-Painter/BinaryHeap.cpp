#include "BinaryHeap.h"

BinaryHeap::BinaryHeap()
{
    elements.resize(1);
    maxSize = currentSize = 0;
}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    for (unsigned i=1; i <= currentSize; i++)
    {
        if (elements[i].uniqueId == uniqueId)
        {
            return false;
        }
    }
    elements[0].uniqueId = uniqueId;
    elements[0].weight = weight;
    if (currentSize == maxSize)
    {
        elements.resize(elements.size()*2);
        maxSize = 2*maxSize+1;
    }
    unsigned hole = ++currentSize;
    for ( ; elements[0].weight < elements[hole/2].weight; hole /= 2)
    {
        elements[hole] = elements[hole/2];
    }
    elements[hole] = elements[0];
    return true;
}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    if (currentSize == 0)
    {
        return false;
    }
    outUniqueId = elements[1].uniqueId;
    outWeight = elements[1].weight;
    elements[1] = elements[currentSize--];
    bubbleDown(1);
    return true;
}

void BinaryHeap::bubbleDown(int hole)
{
    int child;
    HeapElement tmp = elements[(unsigned) hole];
    for ( ; hole * 2 <= currentSize; hole = child)
    {
        child = hole * 2;
        if (child != currentSize && elements[(unsigned) child+1].weight < elements[(unsigned) child].weight)
        {
            child++;
        }
        if ( elements[(unsigned) child].weight < tmp.weight)
        {
            elements[(unsigned) hole] = elements[(unsigned) child];
        }
        else
        {
            break;
        }
    }
    elements[(unsigned) hole] = tmp;
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    for (int i = 1; i <= currentSize; i++)
    {
        if (elements[(unsigned) i].uniqueId == uniqueId)
        {
            elements[(unsigned) i].weight = newWeight;
            if (i/2 >= 1 && elements[(unsigned) i].weight < elements[(unsigned) i/2].weight)
            {
                bubbleUp(i);
            }
            else
            {
                bubbleDown(i);
            }
            return true;
        }
    }
    return false;
}

void BinaryHeap::bubbleUp(int hole)
{
    elements[0] = elements[(unsigned) hole];
    for ( ; elements[0].weight < elements[(unsigned) hole/2].weight; hole /= 2)
    {
        elements[(unsigned) hole] = elements[(unsigned) hole/2];
    }
    elements[(unsigned) hole] = elements[0];
}

int BinaryHeap::HeapSize() const
{
    return (int) currentSize;
}
