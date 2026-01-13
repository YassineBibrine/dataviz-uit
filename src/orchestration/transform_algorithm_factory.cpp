#include "transform_algorithm_factory.h"
#include "../algorithms/normalize.h"
#include "../algorithms/reverse.h"
#include "../algorithms/map_transform.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<Algorithm> TransformAlgorithmFactory::createAlgorithm(const std::string& type) {
  if (type == "Normalize") {
        return std::make_unique<Normalize>();
    }
    else if (type == "Reverse") {
        return std::make_unique<Reverse>();
    }
  else if (type == "Map") {
     return std::make_unique<MapTransform>();
    }
    throw std::invalid_argument("Unknown transform algorithm: " + type);
}
