#pragma once

#include <vector>

struct HeapElement
{
    int     uniqueId;
    double  weight;
};

class BinaryHeap
{
    private:
    std::vector<HeapElement> elements;

    // Do not remove this the tester will utilize this
    // to access the private parts.
    friend class HW3Tester;

    private: // My utility functions / variables
    unsigned maxSize; // Maximum amount of elements the heap can have before resizing
    unsigned currentSize; // The amount of elements currently in the heap
    void bubbleDown(int hole); // For when we need to remove from the heap or ChangePriority breaks the heap
    void bubbleUp(int hole); // For when ChangePriority breaks the heap

    protected:
    public:
        // Constructors & Destructor
        BinaryHeap();
        //
        bool Add(int uniqueId, double weight);
        bool PopHeap(int& outUniqueId, double& outWeight);
        bool ChangePriority(int uniqueId, double newWeight);
        int HeapSize() const;
};
