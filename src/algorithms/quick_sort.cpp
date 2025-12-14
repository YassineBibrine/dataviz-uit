// src/algorithms/quick_sort.cpp
#include "quick_sort.h"
#include <iostream>
#include <algorithm>

QuickSort::QuickSort() {
    // Initialize with sample data
    data = {64, 34, 25, 12, 22, 11, 90};
    originalData = data;
    isSorted = false;
}

QuickSort::QuickSort(ArrayStructure* arrayStructure) {
    loadData(arrayStructure);
    isSorted = false;
}

void QuickSort::loadData(ArrayStructure* arrayStructure) {
    if (!arrayStructure) {
        std::cerr << "Error: ArrayStructure is null" << std::endl;
        return;
    }

    // Get data using ArrayStructure::getDataForRunner()
    void* rawData = arrayStructure->getDataForRunner();
    if (rawData) {
        auto* dataPtr = static_cast<std::vector<int>*>(rawData);
        data = *dataPtr;
        originalData = data;
    } else {
        // Fallback to getData() if getDataForRunner() returns nullptr
        data = arrayStructure->getData();
        originalData = data;
    }
}

void QuickSort::execute() {
    if (data.empty()) {
        std::cout << "? QuickSort: No data to sort" << std::endl;
        return;
    }

    std::cout << "?? Executing QuickSort..." << std::endl;

    executionState.reset();
    frameRecorder.clear();

    quickSortHelper(0, static_cast<int>(data.size()) - 1);

    isSorted = true;

    // Final state
    std::map<std::string, std::string> details;
    details["operation"] = "complete";
    executionState.recordOperation("complete", details);

    // Output result
    std::cout << "? Result: ";
    for (int v : data) std::cout << v << " ";
    std::cout << std::endl;

    // Output metrics
    auto metrics = executionState.getMetrics();
    std::cout << "? Comparisons: " << metrics["comparisons"]
              << " | Swaps: " << metrics["swaps"]
              << " | Total Steps: " << metrics["totalSteps"] << std::endl;
}

int QuickSort::partition(int low, int high) {
    // Choose rightmost element as pivot
    int pivot = data[high];
    int i = low - 1;

    // Record partition start
    std::map<std::string, std::string> partitionDetails;
    partitionDetails["low"] = std::to_string(low);
    partitionDetails["high"] = std::to_string(high);
    partitionDetails["pivot"] = std::to_string(pivot);
    partitionDetails["operation"] = "partition_start";
    executionState.recordOperation("partition_start", partitionDetails);

    for (int j = low; j < high; ++j) {
        // Record comparison
        std::map<std::string, std::string> comparisonDetails;
        comparisonDetails["index1"] = std::to_string(j);
        comparisonDetails["index2"] = std::to_string(high);
        comparisonDetails["value1"] = std::to_string(data[j]);
        comparisonDetails["value2"] = std::to_string(pivot);
        executionState.recordOperation("comparison", comparisonDetails);

        if (data[j] < pivot) {
            i++;
            std::swap(data[i], data[j]);

            // Record swap
            std::map<std::string, std::string> swapDetails;
            swapDetails["index1"] = std::to_string(i);
            swapDetails["index2"] = std::to_string(j);
            executionState.recordOperation("swap", swapDetails);

            // Generate swap frame
            AnimationFrame swapFrame = frameRecorder.generateSwapFrame(
                std::to_string(i),
                std::to_string(j)
            );
            swapFrame.frameNumber = currentStep;
            swapFrame.duration = frameRecorder.defaultDuration;
        }

        executionState.advanceStep();
        currentStep++;
    }

    // Final swap with pivot
    std::swap(data[i + 1], data[high]);

    std::map<std::string, std::string> pivotSwapDetails;
    pivotSwapDetails["index1"] = std::to_string(i + 1);
    pivotSwapDetails["index2"] = std::to_string(high);
    executionState.recordOperation("swap", pivotSwapDetails);

    executionState.advanceStep();
    currentStep++;

    return i + 1;
}

void QuickSort::quickSortHelper(int low, int high) {
    if (low < high) {
        int pi = partition(low, high);

        // Record partition complete
        std::map<std::string, std::string> details;
        details["pivotIndex"] = std::to_string(pi);
        executionState.recordOperation("partition_complete", details);

        // Recursively sort left and right
        quickSortHelper(low, pi - 1);
        quickSortHelper(pi + 1, high);
    }
}

void QuickSort::stepForward() {
    if (isSorted || data.empty()) return;

    // For step-by-step execution, we process one comparison at a time
    // This is a simplified approach - a full implementation would track the recursion stack
    static int low = 0, high = static_cast<int>(data.size()) - 1;
    static bool initialized = false;

    if (!initialized) {
        low = 0;
        high = static_cast<int>(data.size()) - 1;
        initialized = true;
    }

    if (low >= high) {
        isSorted = true;
        return;
    }

    // Execute one partition step
    int pi = partition(low, high);

    if (low < pi - 1) {
        high = pi - 1;
    } else {
        low = pi + 1;
        high = static_cast<int>(data.size()) - 1;
    }
}

void QuickSort::stepBackward() {
    // Reset and replay to previous step
    if (currentStep > 0) {
        int targetStep = currentStep - 1;
        reset();

        // Re-execute up to targetStep
        int steps = 0;
        int low = 0, high = static_cast<int>(data.size()) - 1;

        // Simple approximation: partition once
        if (steps < targetStep && low < high) {
            partition(low, high);
        }
    }
}

void QuickSort::reset() {
    AlgorithmRunner::reset();
    data = originalData;
    isSorted = false;
    currentStep = 0;
    partitionStack.clear();
}
