#ifndef FILTERING_ALGORITHM_FACTORY_H
#define FILTERING_ALGORITHM_FACTORY_H

#include "algorithm_factory.h"
#include <memory>
#include <string>

class FilteringAlgorithmFactory : public AlgorithmFactory {
public:
    std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) override;
};

#endif // FILTERING_ALGORITHM_FACTORY_H
