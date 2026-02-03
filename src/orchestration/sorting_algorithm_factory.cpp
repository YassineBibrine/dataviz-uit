// src/orchestration/sorting_algorithm_factory.cpp
#include "sorting_algorithm_factory.h"
#include "../algorithms/bubble_sort.h"
#include "../algorithms/quick_sort.h"
#include "../algorithms/merge_sort.h"
#include "../algorithms/insertion_sort.h"
#include "../algorithms/selection_sort.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<AlgorithmRunner> SortingAlgorithmFactory::createAlgorithm(
    const std::string& type,
    ArrayStructure* arrayStructure) {

    if (type == "BubbleSort") {
        return std::make_unique<BubbleSort>(arrayStructure);
    }
    else if (type == "QuickSort") {
        return std::make_unique<QuickSort>(arrayStructure);
    }
    else if (type == "MergeSort") {
        return std::make_unique<MergeSort>(arrayStructure);
    }
    else if (type == "InsertionSort") {
        return std::make_unique<InsertionSort>(arrayStructure);
    }
    else if (type == "SelectionSort") {
        return std::make_unique<SelectionSort>(arrayStructure);
    }

    throw std::invalid_argument("Unknown algorithm: " + type);
}