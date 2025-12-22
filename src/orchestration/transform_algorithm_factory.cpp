#include "transform_algorithm_factory.h"
#include "../algorithms/normalize.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<Algorithm> TransformAlgorithmFactory::createAlgorithm(const std::string& type) {
  if (type == "Normalize") {
        return std::make_unique<Normalize>();
    }
    throw std::invalid_argument("Unknown transform algorithm: " + type);
}
