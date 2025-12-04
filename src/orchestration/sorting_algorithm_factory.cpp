// src/orchestration/sorting_algorithm_factory.cpp
#include "sorting_algorithm_factory.h"
#include "../algorithms/bubble_sort.h"
#include "../algorithms/quick_sort.h"
#include "../algorithms/merge_sort.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<Algorithm> SortingAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "BubbleSort") {
        return std::make_unique<BubbleSort>();
    } else if (type == "QuickSort") {
        return std::make_unique<QuickSort>();
    } else if (type == "MergeSort") {
        return std::make_unique<MergeSort>();
    }
    throw std::invalid_argument("Unknown algorithm: " + type);
}