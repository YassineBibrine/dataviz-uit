#ifndef GRAPH_ALGORITHM_FACTORY_H
#define GRAPH_ALGORITHM_FACTORY_H

#include "algorithm_factory.h"
#include <memory>
#include <string>

class GraphAlgorithmFactory : public AlgorithmFactory {
public:
std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) override;
};

#endif // GRAPH_ALGORITHM_FACTORY_H
