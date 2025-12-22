#ifndef TRANSFORM_ALGORITHM_FACTORY_H
#define TRANSFORM_ALGORITHM_FACTORY_H

#include "algorithm_factory.h"
#include <memory>
#include <string>

class TransformAlgorithmFactory : public AlgorithmFactory {
public:
    std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) override;
};

#endif // TRANSFORM_ALGORITHM_FACTORY_H
