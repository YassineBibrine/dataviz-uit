// src/algorithms/bubble_sort.cpp
#include "bubble_sort.h"
#include <iostream>
#include <algorithm>

BubbleSort::BubbleSort() {
    // Initialize with sample data
    data = {64, 34, 25, 12, 22, 11, 90};
    originalData = data;
    isSorted = false;
}

BubbleSort::BubbleSort(ArrayStructure* arrayStructure) {
    loadData(arrayStructure);
    isSorted = false;
}

void BubbleSort::loadData(ArrayStructure* arrayStructure) {
    if (!arrayStructure) {
        std::cerr << "Error: ArrayStructure is null" << std::endl;
        return;
    }

    // Get data using ArrayStructure::getDataForRunner()
    void* rawData = arrayStructure->getDataForRunner();
    if (rawData) {
        // Cast to vector<int>*
        auto* dataPtr = static_cast<std::vector<int>*>(rawData);
        data = *dataPtr;
        originalData = data;
    } else {
        // Fallback to getData() if getDataForRunner() returns nullptr
        data = arrayStructure->getData();
        originalData = data;
    }
}

void BubbleSort::execute() {
    if (data.empty()) {
        std::cout << "? Bubble Sort: No data to sort" << std::endl;
        return;
    }

    std::cout << "?? Executing BubbleSort..." << std::endl;

    size_t n = data.size();
    bool swapped;

    executionState.reset();
    frameRecorder.clear();

    for (size_t i = 0; i < n - 1; ++i) {
        swapped = false;

        for (size_t j = 0; j < n - i - 1; ++j) {
            // Process each comparison
            if (processComparison(i, j)) {
                swapped = true;
            }
        }

        if (!swapped) break;
    }

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

bool BubbleSort::processComparison(size_t i, size_t j) {
    // Record comparison
    std::map<std::string, std::string> comparisonDetails;
    comparisonDetails["index1"] = std::to_string(j);
    comparisonDetails["index2"] = std::to_string(j + 1);
    comparisonDetails["value1"] = std::to_string(data[j]);
    comparisonDetails["value2"] = std::to_string(data[j + 1]);
    executionState.recordOperation("comparison", comparisonDetails);

    // Generate comparison frame
    AnimationFrame compFrame = frameRecorder.generateComparisonFrame(
        std::to_string(j),
        std::to_string(j + 1),
        data[j] > data[j + 1]
    );
    compFrame.frameNumber = currentStep;
    compFrame.duration = frameRecorder.defaultDuration;

    bool swapped = false;

    // Perform comparison and swap
    if (data[j] > data[j + 1]) {
        std::swap(data[j], data[j + 1]);
        swapped = true;

        // Record swap operation
        std::map<std::string, std::string> swapDetails;
        swapDetails["index1"] = std::to_string(j);
        swapDetails["index2"] = std::to_string(j + 1);
        executionState.recordOperation("swap", swapDetails);

        // Generate swap frame
        AnimationFrame swapFrame = frameRecorder.generateSwapFrame(
            std::to_string(j),
            std::to_string(j + 1)
        );
        swapFrame.frameNumber = currentStep;
        swapFrame.duration = frameRecorder.defaultDuration;
    }

    executionState.advanceStep();
    currentStep++;

    return swapped;
}

void BubbleSort::stepForward() {
    if (isSorted || data.empty()) return;

    static size_t i = 0, j = 0;
    static bool initialized = false;

    if (!initialized) {
        i = 0;
        j = 0;
        initialized = true;
    }

    size_t n = data.size();

    if (i >= n - 1) {
        isSorted = true;
        return;
    }

    if (j >= n - i - 1) {
        j = 0;
        i++;
        return;
    }

    processComparison(i, j);
    j++;
}

void BubbleSort::stepBackward() {
    // Reset and replay to previous step
    if (currentStep > 0) {
        int targetStep = currentStep - 1;
        reset();

        // Re-execute up to targetStep
        size_t n = data.size();
        int steps = 0;

        for (size_t i = 0; i < n - 1 && steps < targetStep; ++i) {
            for (size_t j = 0; j < n - i - 1 && steps < targetStep; ++j) {
                processComparison(i, j);
                steps++;
            }
        }
    }
}

void BubbleSort::reset() {
    AlgorithmRunner::reset();
    data = originalData;
    isSorted = false;
    currentStep = 0;
}