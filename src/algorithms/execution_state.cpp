#include "execution_state.h"

void ExecutionState::recordComparison(const std::string& /*elem1*/, const std::string& /*elem2*/) {
    ++comparisonCount;
    ++currentStep;
}

void ExecutionState::recordSwap(const std::string& /*elem1*/, const std::string& /*elem2*/) {
    ++swapCount;
    ++currentStep;
}

void ExecutionState::recordAccess(const std::string& /*elem*/) {
    ++accessCount_;
    ++currentStep;
}

void ExecutionState::recordOperation(const std::string& /*operation*/, const std::map<std::string, std::string>& /*details*/) {
    ++currentStep;
}

void ExecutionState::advanceStep() {
    ++currentStep;
}

void ExecutionState::reset() {
    currentStep = 0;
    comparisonCount = 0;
    swapCount = 0;
    accessCount_ = 0;
    totalSteps = 0;
    stateHistory.clear();
}

void ExecutionState::saveState() {
    ExecutionSnapshot snap;
    snap.stepNumber = currentStep;
    snap.comparisonCount = comparisonCount;
    snap.swapCount = swapCount;
    snap.metrics = getMetrics();
    stateHistory.push_back(snap);
    totalSteps = std::max(totalSteps, currentStep);
}

std::map<std::string, int> ExecutionState::getMetrics() const {
    return {
        {"currentStep", currentStep},
        {"totalSteps", totalSteps},
        {"comparisons", comparisonCount},
        {"swaps", swapCount},
        {"accesses", accessCount_}
    };
}

void ExecutionState::restoreState(int stepNumber) {
    for (const auto& s : stateHistory) {
        if (s.stepNumber == stepNumber) {
            currentStep = s.stepNumber;
            comparisonCount = s.comparisonCount;
            swapCount = s.swapCount;
            // accessCount not recorded in snapshot metrics; optional recovery:
            auto it = s.metrics.find("accesses");
            if (it != s.metrics.end()) {
                accessCount_ = it->second;
            }
            return;
        }
    }
    // If not found, no-op
}
