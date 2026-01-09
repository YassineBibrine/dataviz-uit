#pragma once
#include <vector>
#include <string>
#include <map>

struct ExecutionSnapshot {
    int stepNumber{};
    int comparisonCount{};
    int swapCount{};
    std::map<std::string, int> metrics;
};

class ExecutionState {
public:
    // Attributes per UML
    int currentStep{ 0 };
    int totalSteps{ 0 };
    int comparisonCount{ 0 };
    int swapCount{ 0 };
    std::vector<ExecutionSnapshot> stateHistory;

    // Methods per UML
    void recordComparison(const std::string& elem1, const std::string& elem2);
    void recordSwap(const std::string& elem1, const std::string& elem2);
    void recordAccess(const std::string& elem);
    void saveState();
    std::map<std::string, int> getMetrics() const;
    void restoreState(int stepNumber);

    // Additional methods
    void recordOperation(const std::string& operation, const std::map<std::string, std::string>& details);
    void advanceStep();
    void reset();

private:
    // Internal counters
    int accessCount_{ 0 };
};
