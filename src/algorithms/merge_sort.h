#pragma once

#include "algorithm_runner.h"
#include "../core/array_structure.h"
#include <vector>
#include <memory>

/**
 * @class MergeSort
 * @brief MergeSort algorithm runner
 * Inherits from AlgorithmRunner to track execution state and generate animation frames
 */
class MergeSort : public AlgorithmRunner {
private:
    std::vector<int> data;
    std::vector<int> originalData;
    std::vector<int> tempData;
    bool isSorted{false};

public:
    /**
     * @brief Default constructor with sample data
     */
    MergeSort();

    /**
     * @brief Constructor with ArrayStructure
     * @param arrayStructure The array data structure to sort
     */
    explicit MergeSort(ArrayStructure* arrayStructure);

    ~MergeSort() override = default;

    /**
     * @brief Execute full MergeSort algorithm
     */
    void execute() override;

    /**
     * @brief Step forward one merge operation
     */
    void stepForward() override;

    /**
     * @brief Step backward one merge operation
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
     * @brief Merge two sorted subarrays
     * @param left Starting index of left subarray
     * @param mid Ending index of left subarray
     * @param right Ending index of right subarray
     */
    void merge(int left, int mid, int right);

    /**
     * @brief MergeSort recursive implementation
     * @param left Starting index
     * @param right Ending index
     */
    void mergeSortHelper(int left, int right);

    /**
     * @brief Load data from ArrayStructure
     * @param arrayStructure The array structure containing data
     */
    void loadData(ArrayStructure* arrayStructure);
};
