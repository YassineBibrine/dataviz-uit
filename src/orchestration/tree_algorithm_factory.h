#ifndef TREE_ALGORITHM_FACTORY_H
#define TREE_ALGORITHM_FACTORY_H

#include "algorithm_factory.h"
#include <memory>
#include <string>

class TreeAlgorithmFactory : public AlgorithmFactory {
public:
    std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) override;
};

#endif // TREE_ALGORITHM_FACTORY_H
