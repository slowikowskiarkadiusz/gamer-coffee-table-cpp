#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

class max_heap {
    std::vector<int> heap;
    size_t capacity;

public:
    explicit max_heap(size_t cap) : capacity(cap) {
    }

    void insert(int value) {
        heap.push_back(value);
        heapifyUp();

        if (heap.size() > capacity) {
            int minIndex = findMinIndex();
            heap.erase(heap.begin() + minIndex);
            heapify();
        }
    }

    int peek() const {
        if (heap.empty()) return 0;
        return heap[0];
    }

    std::vector<int> values() const {
        std::vector<int> sorted = heap;
        std::sort(sorted.begin(), sorted.end(), std::greater<int>());
        return sorted;
    }

private:
    void heapify() {
        for (int i = static_cast<int>(heap.size() / 2) - 1; i >= 0; --i) {
            heapifyDown(i);
        }
    }

    void heapifyUp() {
        int index = static_cast<int>(heap.size()) - 1;
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] <= heap[parent]) break;
            std::swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int length = static_cast<int>(heap.size());

        while (true) {
            int largest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < length && heap[left] > heap[largest]) largest = left;
            if (right < length && heap[right] > heap[largest]) largest = right;

            if (largest == index) break;

            std::swap(heap[index], heap[largest]);
            index = largest;
        }
    }

    int findMinIndex() const {
        int min = heap[0];
        int minIndex = 0;
        for (int i = 1; i < heap.size(); ++i) {
            if (heap[i] < min) {
                min = heap[i];
                minIndex = i;
            }
        }
        return minIndex;
    }
};
