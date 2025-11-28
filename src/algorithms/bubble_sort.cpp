// src/algorithms/bubble_sort.cpp
#include "bubble_sort.h"
#include <iostream>
#include <vector>

BubbleSort::BubbleSort() = default;

void BubbleSort::execute() {
    std::vector<int> data = { 64, 34, 25, 12, 22, 11, 90 };
    std::cout << "?? Executing BubbleSort..." << std::endl;

    // Bubble Sort
    bool swapped;
    size_t n = data.size();
    for (size_t i = 0; i < n - 1; ++i) {
        swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }

    std::cout << "? Result: ";
    for (int v : data) std::cout << v << " ";
    std::cout << std::endl;
}