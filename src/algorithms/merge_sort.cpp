// src/algorithms/merge_sort.cpp
#include "merge_sort.h"
#include <iostream>
#include <algorithm>

MergeSort::MergeSort() {
    // Initialize with sample data
    data = {64, 34, 25, 12, 22, 11, 90};
    originalData = data;
    tempData.resize(data.size());
    isSorted = false;
}

MergeSort::MergeSort(ArrayStructure* arrayStructure) {
    loadData(arrayStructure);
    tempData.resize(data.size());
    isSorted = false;
}

void MergeSort::loadData(ArrayStructure* arrayStructure) {
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

    tempData.resize(data.size());
}

void MergeSort::execute() {
    if (data.empty()) {
        std::cout << "? MergeSort: No data to sort" << std::endl;
        return;
    }

    std::cout << "?? Executing MergeSort..." << std::endl;

    executionState.reset();
    frameRecorder.clear();

    mergeSortHelper(0, static_cast<int>(data.size()) - 1);

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

void MergeSort::merge(int left, int mid, int right) {
    int leftIdx = left;
    int rightIdx = mid + 1;
    int tempIdx = left;

    // Record merge start
    std::map<std::string, std::string> mergeDetails;
    mergeDetails["left"] = std::to_string(left);
    mergeDetails["mid"] = std::to_string(mid);
    mergeDetails["right"] = std::to_string(right);
    mergeDetails["operation"] = "merge_start";
    executionState.recordOperation("merge_start", mergeDetails);

    // Merge the two sorted subarrays
    while (leftIdx <= mid && rightIdx <= right) {
        // Record comparison
        std::map<std::string, std::string> comparisonDetails;
        comparisonDetails["index1"] = std::to_string(leftIdx);
        comparisonDetails["index2"] = std::to_string(rightIdx);
        comparisonDetails["value1"] = std::to_string(data[leftIdx]);
        comparisonDetails["value2"] = std::to_string(data[rightIdx]);
        executionState.recordOperation("comparison", comparisonDetails);

        if (data[leftIdx] <= data[rightIdx]) {
            tempData[tempIdx++] = data[leftIdx++];
        } else {
            tempData[tempIdx++] = data[rightIdx++];
        }

        executionState.advanceStep();
        currentStep++;
    }

    // Copy remaining elements from left subarray
    while (leftIdx <= mid) {
        tempData[tempIdx++] = data[leftIdx++];
        executionState.advanceStep();
        currentStep++;
    }

    // Copy remaining elements from right subarray
    while (rightIdx <= right) {
        tempData[tempIdx++] = data[rightIdx++];
        executionState.advanceStep();
        currentStep++;
    }

    // Copy merged elements back to data array
    for (int i = left; i <= right; ++i) {
        data[i] = tempData[i];

        // Record copy operation
        std::map<std::string, std::string> copyDetails;
        copyDetails["index"] = std::to_string(i);
        copyDetails["value"] = std::to_string(data[i]);
        executionState.recordOperation("copy", copyDetails);

        // Generate highlight frame
        AnimationFrame highlightFrame = frameRecorder.generateHighlightFrame(
            {std::to_string(i)},
            "green"
        );
        highlightFrame.frameNumber = currentStep;
        highlightFrame.duration = frameRecorder.defaultDuration;

        executionState.advanceStep();
        currentStep++;
    }

    // Record merge complete
    std::map<std::string, std::string> completeDetails;
    completeDetails["operation"] = "merge_complete";
    executionState.recordOperation("merge_complete", completeDetails);
}

void MergeSort::mergeSortHelper(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Record divide operation
        std::map<std::string, std::string> divideDetails;
        divideDetails["left"] = std::to_string(left);
        divideDetails["mid"] = std::to_string(mid);
        divideDetails["right"] = std::to_string(right);
        divideDetails["operation"] = "divide";
        executionState.recordOperation("divide", divideDetails);

        // Recursively sort left and right halves
        mergeSortHelper(left, mid);
        mergeSortHelper(mid + 1, right);

        // Merge the sorted halves
        merge(left, mid, right);
    } else if (left == right) {
        // Base case: single element is already sorted
        std::map<std::string, std::string> baseDetails;
        baseDetails["index"] = std::to_string(left);
        baseDetails["operation"] = "base_case";
        executionState.recordOperation("base_case", baseDetails);
    }
}

void MergeSort::stepForward() {
    if (isSorted || data.empty()) return;

    // For step-by-step execution, merge one pair of elements
    // This is simplified - full implementation would track recursion stack
    static int left = 0, mid = 0, right = 1;
    static bool initialized = false;

    if (!initialized) {
        left = 0;
        mid = 0;
        right = 1;
        initialized = true;
    }

    if (right >= static_cast<int>(data.size())) {
        isSorted = true;
        return;
    }

    if (mid < right) {
        merge(left, mid, right);
        left++;
        mid++;
        right++;
    }
}

void MergeSort::stepBackward() {
    // Reset and replay to previous step
    if (currentStep > 0) {
        int targetStep = currentStep - 1;
        reset();

        // Re-execute up to targetStep - limited approximation
        if (targetStep > 0) {
            mergeSortHelper(0, std::min(1, static_cast<int>(data.size()) - 1));
        }
    }
}

void MergeSort::reset() {
    AlgorithmRunner::reset();
    data = originalData;
    isSorted = false;
    currentStep = 0;
    tempData.resize(data.size());
}
