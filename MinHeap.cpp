#include "MinHeap.h"
#include <stdexcept>

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].first < heap[parent].first) {
            std::swap(heap[index], heap[parent]);
            index = parent;
        } else break;
    }
}

void MinHeap::heapifyDown(int index) {
    int size = heap.size();
    while (index < size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && heap[left].first < heap[smallest].first)
            smallest = left;
        if (right < size && heap[right].first < heap[smallest].first)
            smallest = right;

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            index = smallest;
        } else break;
    }
}

void MinHeap::insert(std::pair<int, int> value) {
    heap.push_back(value);
    heapifyUp(heap.size() - 1);
}

std::pair<int, int> MinHeap::extractMin() {
    if (heap.empty()) throw std::runtime_error("Heap is empty");
    std::pair<int, int> minVal = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);
    return minVal;
}

std::pair<int, int> MinHeap::peek() const {
    if (heap.empty()) throw std::runtime_error("Heap is empty");
    return heap[0];
}

bool MinHeap::isEmpty() const {
    return heap.empty();
}
