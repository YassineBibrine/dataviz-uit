#include "graph_algorithm_factory.h"
#include "../algorithms/graph_algorithms.h"
#include <stdexcept>

std::unique_ptr<Algorithm> GraphAlgorithmFactory::createAlgorithm(const std::string& type) {
    if (type == "BFS") {
        return std::make_unique<BFSAlgorithm>();
    } else if (type == "DFS") {
        return std::make_unique<DFSAlgorithm>();
    } else if (type == "Dijkstra") {
        return std::make_unique<DijkstraAlgorithm>();
    }

    throw std::invalid_argument("Unknown graph algorithm type: " + type);
}
