// src/orchestration/sorting_algorithm_factory.cpp
#include "sorting_algorithm_factory.h"
#include "../algorithms/bubble_sort.h"
#include "../algorithms/quick_sort.h"
#include "../algorithms/merge_sort.h"
#include "../core/array_structure.h"
#include <memory>
#include <stdexcept>
#include "../algorithms/inorder_runner.h"
#include "../algorithms/preorder_runner.h"
#include "../algorithms/postorder_runner.h"
#include <stdexcept>

std::unique_ptr<Algorithm> SortingAlgorithmFactory::createAlgorithm(const std::string& type) {
    // Default constructor for backward compatibility
    if (type == "BubbleSort") {
        return std::make_unique<BubbleSort>();
    } else if (type == "QuickSort") {
        return std::make_unique<QuickSort>();
    } else if (type == "MergeSort") {
        return std::make_unique<MergeSort>();
    }
    throw std::invalid_argument("Unknown algorithm: " + type);
}

std::unique_ptr<Algorithm> SortingAlgorithmFactory::createAlgorithmWithData(
    const std::string& type, ArrayStructure* data) {
    
    if (!data) {
        throw std::invalid_argument("ArrayStructure pointer is null");
    }
    
    if (type == "BubbleSort") {
        return std::make_unique<BubbleSort>(data);
    } else if (type == "QuickSort") {
        return std::make_unique<QuickSort>(data);
    } else if (type == "MergeSort") {
        return std::make_unique<MergeSort>(data);
    }
    throw std::invalid_argument("Unknown algorithm: " + type);
}

std::unique_ptr<AlgorithmRunner> TreeAlgorithmFactory::createAlgorithm(
    const std::string& type, TreeStructure* tree) {

    if (!tree) {
        throw std::invalid_argument("TreeStructure pointer is null");
    }

    if (type == "Inorder") {
        return std::make_unique<InorderRunner>(tree);
    }
    else if (type == "Preorder") {
        return std::make_unique<PreorderRunner>(tree);
    }
    else if (type == "Postorder") {
        return std::make_unique<PostorderRunner>(tree);
    }

    throw std::invalid_argument("Unknown tree algorithm: " + type);
}
