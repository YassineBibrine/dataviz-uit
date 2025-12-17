#pragma once

#include "algorithm_runner.h"
#include "../core/array_structure.h"
#include <vector>
#include <memory>

class Normalize : public AlgorithmRunner {
private:
    std::vector<double> data;
  std::vector<double> originalData;
    double minValue{0.0};
    double maxValue{1.0};
    bool isNormalized{false};

public:
    Normalize();
    explicit Normalize(ArrayStructure* arrayStructure);
    ~Normalize() override = default;

    void execute() override;
    void stepForward() override;
    void stepBackward() override;
    void reset() override;

 const std::vector<double>& getNormalizedData() const { return data; }
    bool isNormalizationComplete() const { return isNormalized; }

private:
    void loadData(ArrayStructure* arrayStructure);
    void calculateMinMax();
};
