#include "normalize.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Normalize::Normalize() : AlgorithmRunner() {
    data = {10.0, 20.0, 30.0, 40.0, 50.0};
    originalData = data;
    calculateMinMax();
}

Normalize::Normalize(ArrayStructure* arrayStructure) : AlgorithmRunner() {
    loadData(arrayStructure);
    calculateMinMax();
}

void Normalize::execute() {
    calculateMinMax();
    
    if (maxValue == minValue) {
        std::fill(data.begin(), data.end(), 0.0);
    } else {
      for (auto& value : data) {
      value = (value - minValue) / (maxValue - minValue);
        }
    }
    
    isNormalized = true;
}

void Normalize::stepForward() {
    if (currentStep < static_cast<int>(data.size())) {
        if (maxValue != minValue) {
      data[currentStep] = (originalData[currentStep] - minValue) / (maxValue - minValue);
        }
        currentStep++;
    }
}

void Normalize::stepBackward() {
    if (currentStep > 0) {
   currentStep--;
        data[currentStep] = originalData[currentStep];
    }
}

void Normalize::reset() {
    data = originalData;
 isNormalized = false;
    currentStep = 0;
    executionState.reset();
    frameRecorder.reset();
}

void Normalize::loadData(ArrayStructure* arrayStructure) {
    if (arrayStructure) {
        data.clear();
        originalData.clear();
    }
}

void Normalize::calculateMinMax() {
    if (data.empty()) return;
    minValue = *std::min_element(data.begin(), data.end());
    maxValue = *std::max_element(data.begin(), data.end());
}
