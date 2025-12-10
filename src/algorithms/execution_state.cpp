#include "execution_state.h"
#include <algorithm> // std::max

// ===============================
// Comparisons
// ===============================
void ExecutionState::recordComparison(const std::string& a, const std::string& b)
{
    (void)a;
    (void)b;
    comparisonCount++;
    metricsMap["comparisons"] = comparisonCount;
}

// ===============================
// Swaps
// ===============================
void ExecutionState::recordSwap(const std::string& a, const std::string& b)
{
    (void)a;
    (void)b;
    swapCount++;
    metricsMap["swaps"] = swapCount;
}

// ===============================
// Simple Access (read/write)
// ===============================
void ExecutionState::recordAccess(const std::string& a)
{
    (void)a;
    metricsMap["access"]++;
}

// ===============================
// Generic Operation Recording
// ===============================
void ExecutionState::recordOperation(const std::string& operation, const std::map<std::string,std::string>& details)
{
    // dynamic counter for the operation
    metricsMap[operation]++;

    // update dedicated counters if present
    if (operation == "comparisons") {
        comparisonCount = metricsMap[operation];
    } else if (operation == "swaps") {
        swapCount = metricsMap[operation];
    }

    // save snapshot directly with details (optional)
    history.push_back({ currentStep, metricsMap, details });
    totalSteps = std::max(totalSteps, currentStep);
}

// ===============================
// Save Complete Snapshot
// ===============================
void ExecutionState::saveState()
{
    history.push_back({ currentStep, metricsMap, {} });
    totalSteps = std::max(totalSteps, currentStep);
}

// ===============================
// Restore Previous State
// ===============================
void ExecutionState::restoreState(int step)
{
    if (step < 0 || step >= static_cast<int>(history.size()))
        return;

    currentStep = step;

    // restore metrics from history
    metricsMap = history[step].metrics;

    // update quick access fields
    auto itComp = metricsMap.find("comparisons");
    comparisonCount = (itComp != metricsMap.end()) ? itComp->second : 0;

    auto itSwap = metricsMap.find("swaps");
    swapCount = (itSwap != metricsMap.end()) ? itSwap->second : 0;
}

// ===============================
// Return All Important Metrics
// ===============================
std::map<std::string,int> ExecutionState::getMetrics() const
{
    auto copy = metricsMap;
    copy["step"] = currentStep;
    copy["totalSteps"] = totalSteps;
    return copy;
}

int ExecutionState::getCurrentStep() const
{
    return currentStep;
}

int ExecutionState::getTotalSteps() const
{
    return totalSteps;
}

// ===============================
// Advance One Step
// ===============================
void ExecutionState::advanceStep()
{
    ++currentStep;
    totalSteps = std::max(totalSteps, currentStep);
}

// ===============================
// Total Reset
// ===============================
void ExecutionState::reset()
{
    currentStep = 0;
    totalSteps = 0;
    comparisonCount = 0;
    swapCount = 0;

    metricsMap.clear();
    metricsMap["comparisons"] = 0;
    metricsMap["swaps"] = 0;
    metricsMap["access"] = 0;

    history.clear();
}
