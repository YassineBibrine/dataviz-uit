#include "range_filter.h"
#include <algorithm>

RangeFilter::RangeFilter() : AlgorithmRunner() {
    data = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95};
    originalData = data;
    minRange = 20;
    maxRange = 80;
}

RangeFilter::RangeFilter(ArrayStructure* arrayStructure) : AlgorithmRunner() {
 loadData(arrayStructure);
}

void RangeFilter::execute() {
    filteredData.clear();
    for (const auto& value : data) {
        if (value >= minRange && value <= maxRange) {
      filteredData.push_back(value);
        }
    }
    isFiltered = true;
}

void RangeFilter::stepForward() {
    if (currentStep < static_cast<int>(data.size())) {
    if (data[currentStep] >= minRange && data[currentStep] <= maxRange) {
 filteredData.push_back(data[currentStep]);
        }
     currentStep++;
    }
}

void RangeFilter::stepBackward() {
  if (currentStep > 0) {
        currentStep--;
     if (!filteredData.empty() && data[currentStep] >= minRange && data[currentStep] <= maxRange) {
      filteredData.pop_back();
        }
    }
}

void RangeFilter::reset() {
    data = originalData;
    filteredData.clear();
    isFiltered = false;
    currentStep = 0;
    executionState.reset();
frameRecorder.reset();
}

void RangeFilter::loadData(ArrayStructure* arrayStructure) {
    if (arrayStructure) {
        data.clear();
originalData.clear();
    }
}
