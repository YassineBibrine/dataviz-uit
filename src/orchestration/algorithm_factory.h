// src/orchestration/algorithm_factory.h
// Abstract factory interface used to create Algorithm instances.
#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include "algorithm.h"
#include <memory>
#include <string>

/**
 * @class AlgorithmFactory
 * @brief Abstract factory used by AlgorithmManager to create algorithm objects.
 *
 * Concrete factories implement createAlgorithm(type) to produce algorithm
 * instances for the given type name. This decouples AlgorithmManager from
 * concrete algorithm classes and allows plugins or extensions to register
 * their own factories.
 */
class AlgorithmFactory {
public:
 virtual ~AlgorithmFactory() = default;
 virtual std::unique_ptr<Algorithm> createAlgorithm(const std::string& type) =0;
};

#endif // ALGORITHM_FACTORY_H