#include "transform_algorithm_factory.h"
#include "../algorithms/reverse.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<Algorithm> TransformAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "Reverse") {
        return std::make_unique<Reverse>();
    }

    throw std::invalid_argument("Unknown transform algorithm: " + type);
}