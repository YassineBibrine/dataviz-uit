#pragma once

#include "algorithm_runner.h"
#include "../core/array_structure.h"
#include <vector>
#include <memory>

/**
 * @class BubbleSort
 * @brief Bubble Sort algorithm runner
 * Inherits from AlgorithmRunner to track execution state and generate animation frames
 */
class BubbleSort : public AlgorithmRunner {
private:
    std::vector<int> data;
    std::vector<int> originalData;
    bool isSorted{false};

public:
    /**
     * @brief Default constructor with sample data
     */
    BubbleSort();

    /**
     * @brief Constructor with ArrayStructure
     * @param arrayStructure The array data structure to sort
     */
    explicit BubbleSort(ArrayStructure* arrayStructure);

    ~BubbleSort() override = default;

    /**
     * @brief Execute full bubble sort algorithm
     */
    void execute() override;

    /**
     * @brief Step forward one comparison/swap
     */
    void stepForward() override;

    /**
     * @brief Step backward one comparison/swap
     */
    void stepBackward() override;

    /**
     * @brief Reset the algorithm to initial state
     */
    void reset() override;

    /**
     * @brief Get the sorted data
     */
    const std::vector<int>& getSortedData() const { return data; }

    /**
     * @brief Check if sorting is complete
     */
    bool isSortingComplete() const { return isSorted; }

private:
    /**
     * @brief Process one comparison and potential swap
     * @param i Outer loop index
     * @param j Inner loop index
     * @return true if a swap occurred
     */
    bool processComparison(size_t i, size_t j);

    /**
     * @brief Load data from ArrayStructure
     * @param arrayStructure The array structure containing data
     */
    void loadData(ArrayStructure* arrayStructure);
};