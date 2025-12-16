#pragma once

#include "algorithm_runner.h"
#include "../core/array_structure.h"
#include <vector>
#include <memory>

/**
 * @class QuickSort
 * @brief QuickSort algorithm runner
 * Inherits from AlgorithmRunner to track execution state and generate animation frames
 */
class QuickSort : public AlgorithmRunner {
private:
    std::vector<int> data;
    std::vector<int> originalData;
    bool isSorted{false};
    
    // For step-by-step execution
    struct PartitionState {
        int low;
        int high;
        int pivotIndex;
    };
    std::vector<PartitionState> partitionStack;

public:
    /**
     * @brief Default constructor with sample data
     */
    QuickSort();

    /**
     * @brief Constructor with ArrayStructure
     * @param arrayStructure The array data structure to sort
     */
    explicit QuickSort(ArrayStructure* arrayStructure);

    ~QuickSort() override = default;

    /**
     * @brief Execute full QuickSort algorithm
     */
    void execute() override;

    /**
     * @brief Step forward one partition operation
     */
    void stepForward() override;

    /**
     * @brief Step backward one partition operation
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
     * @brief Partition array and return pivot index
     * @param low Starting index
     * @param high Ending index
     * @return Pivot index after partition
     */
    int partition(int low, int high);

    /**
     * @brief QuickSort recursive implementation
     * @param low Starting index
     * @param high Ending index
     */
    void quickSortHelper(int low, int high);

    /**
     * @brief Load data from ArrayStructure
     * @param arrayStructure The array structure containing data
     */
    void loadData(ArrayStructure* arrayStructure);
};
