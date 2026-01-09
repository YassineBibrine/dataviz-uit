// src/orchestration/sorting_algorithm_factory.h
#ifndef SORTING_ALGORITHM_FACTORY_H
#define SORTING_ALGORITHM_FACTORY_H

#include "algorithm_factory.h"
#include <memory>
#include <string>
#include "../core/tree_structure.h"
#include "../algorithms/algorithm_runner.h"

// Forward declarations
class Algorithm;
class ArrayStructure;

class SortingAlgorithmFactory : public AlgorithmFactory {
public:
    std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) override;
    std::unique_ptr<Algorithm> createAlgorithmWithData(const std::string& type, ArrayStructure* data);
};

class TreeAlgorithmFactory {
public:
    static std::unique_ptr<AlgorithmRunner> createAlgorithm(const std::string& type, TreeStructure* tree);
};

#endif // SORTING_ALGORITHM_FACTORY_H