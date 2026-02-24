#ifndef SORTING_ALGORITHM_FACTORY_H
#define SORTING_ALGORITHM_FACTORY_H

#include "algorithm_factory.h"
#include <memory>
#include <string>

/**
 * @class SortingAlgorithmFactory
 * @brief Factory that creates sorting algorithm instances used by AlgorithmManager.
 *
 * Supported types include: InsertionSort, SelectionSort, BubbleSort.
 */
class SortingAlgorithmFactory : public AlgorithmFactory {
public:
    std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) override;
};

#endif // SORTING_ALGORITHM_FACTORY_H
