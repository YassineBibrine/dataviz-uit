#pragma once

#include "algorithm_runner.h"
#include "../core/array_structure.h"
#include <vector>
#include <memory>

class RangeFilter : public AlgorithmRunner {
private:
    std::vector<int> data;
    std::vector<int> originalData;
    std::vector<int> filteredData;
    int minRange{0};
    int maxRange{100};
    bool isFiltered{false};

public:
    RangeFilter();
    explicit RangeFilter(ArrayStructure* arrayStructure);
    ~RangeFilter() override = default;

    void execute() override;
    void stepForward() override;
    void stepBackward() override;
  void reset() override;

    void setRange(int min, int max) {
        minRange = min;
   maxRange = max;
    }

    const std::vector<int>& getFilteredData() const { return filteredData; }
    bool isFilteringComplete() const { return isFiltered; }

private:
    void loadData(ArrayStructure* arrayStructure);
};
