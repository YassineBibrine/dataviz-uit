// src/orchestration/sorting_algorithm_factory.h
#ifndef SORTING_ALGORITHM_FACTORY_H
#define SORTING_ALGORITHM_FACTORY_H

#include "../algorithms/algorithm_runner.h"
#include "../core/array_structure.h"
#include <memory>
#include <string>

class SortingAlgorithmFactory {
public:
    static std::unique_ptr<AlgorithmRunner> createAlgorithm(
        const std::string& type,
        ArrayStructure* arrayStructure
    );
};

#endif // SORTING_ALGORITHM_FACTORY_H