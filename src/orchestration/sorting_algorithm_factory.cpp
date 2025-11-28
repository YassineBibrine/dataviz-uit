// src/orchestration/sorting_algorithm_factory.cpp
#include "sorting_algorithm_factory.h"
#include "algorithms/bubble_sort.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<Algorithm> SortingAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "BubbleSort") {
        return std::make_unique<BubbleSort>();
    }
    throw std::invalid_argument("Unknown algorithm: " + type);
}