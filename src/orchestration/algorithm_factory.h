// src/orchestration/algorithm_factory.h
#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "algorithm.h"
#include <memory>
#include <string>

class AlgorithmFactory {
public:
    virtual ~AlgorithmFactory() = default;
    virtual std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) = 0;
};

#endif // ALGORITHM_FACTORY_H