// src/orchestration/sorting_algorithm_factory.cpp
#include "sorting_algorithm_factory.h"
#include "../algorithms/bubble_sort.h"
#include "../algorithms/quick_sort.h"
#include "../algorithms/merge_sort.h"
#include "../algorithms/insertion_sort.h"
#include "../algorithms/selection_sort.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<Algorithm> SortingAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "BubbleSort") {
        return std::make_unique<BubbleSort>();
    } else if (type == "QuickSort") {
        return std::make_unique<QuickSort>();
    } else if (type == "MergeSort") {
        return std::make_unique<MergeSort>();
    } else if (type == "InsertionSort") {
        return std::make_unique<InsertionSort>();
    } else if (type == "SelectionSort") {
        return std::make_unique<SelectionSort>();
    }
    throw std::invalid_argument("Unknown algorithm: " + type);
}