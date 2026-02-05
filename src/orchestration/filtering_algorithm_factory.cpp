#include "filtering_algorithm_factory.h"
#include "../algorithms/remove_duplicates.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<Algorithm> FilteringAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "RemoveDuplicates") {
        return std::make_unique<RemoveDuplicates>();
    }

    throw std::invalid_argument("Unknown filtering algorithm: " + type);
}