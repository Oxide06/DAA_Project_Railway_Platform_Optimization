#pragma once
#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include <utility>

class MinHeap {
private:
    std::vector<std::pair<int, int>> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    void insert(std::pair<int, int> value);
    std::pair<int, int> extractMin();
    std::pair<int, int> peek() const;
    bool isEmpty() const;
};

#endif
