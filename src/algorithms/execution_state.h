#pragma once

#include <map>
#include <string>
#include <vector>

struct ExecutionSnapshot {
 int step;
 std::map<std::string,int> metrics;
};

class ExecutionState {
 int currentStep{0};
 int totalSteps{0};
 int comparisonCount{0};
 int swapCount{0};
 std::vector<ExecutionSnapshot> history;
public:
 void recordComparison(const std::string& a, const std::string& b) {
 (void)a; (void)b; comparisonCount++; }
 void recordSwap(const std::string& a, const std::string& b) {
 (void)a; (void)b; swapCount++; }
 void recordAccess(const std::string& a) { (void)a; }
 void saveState() {
 history.push_back({currentStep, getMetrics()});
 totalSteps = std::max(totalSteps, currentStep);
 }
 std::map<std::string,int> getMetrics() const {
 return {{"comparisons", comparisonCount},{"swaps", swapCount},{"step", currentStep},{"totalSteps", totalSteps}}; }
 void restoreState(int step) { currentStep = step; }
 void reset() { currentStep=0; totalSteps=0; comparisonCount=0; swapCount=0; history.clear(); }
};
