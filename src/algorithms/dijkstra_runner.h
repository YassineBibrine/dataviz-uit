#pragma once

#include "algorithm_runner.h"
#include "../core/graph.h"
#include <map>
#include <set>
#include <vector>
#include <limits>
#include <queue>

/**
 * @class DijkstraRunner
 * @brief Dijkstra's shortest path algorithm runner
 * Records all steps and metrics for visualization and playback
 */
class DijkstraRunner : public AlgorithmRunner {
private:
    Graph* graph;
    std::string startNodeId;
    
    // Dijkstra state
    std::map<std::string, double> distances;
    std::map<std::string, std::string> previousNode;
    std::set<std::string> visited;
    std::set<std::string> unvisited;
    
    std::vector<std::string> processedOrder;
    std::vector<std::pair<std::string, std::string>> shortestPathEdges;
    
    bool isExecuted{false};
    const double INFINITY_DIST = std::numeric_limits<double>::max();

public:
    DijkstraRunner(Graph* graph, const std::string& startNodeId);
    ~DijkstraRunner() override = default;

    /**
     * @brief Execute full Dijkstra's algorithm
     */
    void execute() override;

    /**
     * @brief Step forward one node processing
     */
    void stepForward() override;

    /**
     * @brief Step backward one node processing
     */
    void stepBackward() override;

    /**
     * @brief Reset the algorithm state
     */
    void reset() override;

    /**
     * @brief Get shortest distance to a node
     */
    double getShortestDistance(const std::string& nodeId) const;

    /**
     * @brief Get shortest path to a destination node
     */
    std::vector<std::string> getShortestPath(const std::string& destinationId) const;

    /**
     * @brief Get all distances from start node
     */
    std::map<std::string, double> getAllDistances() const { return distances; }

    /**
     * @brief Get processed nodes in order
     */
    std::vector<std::string> getProcessedOrder() const { return processedOrder; }

    /**
     * @brief Get shortest path edges discovered
     */
    std::vector<std::pair<std::string, std::string>> getShortestPathEdges() const { return shortestPathEdges; }

private:
    /**
     * @brief Process one node (find unvisited node with minimum distance)
     */
    void processStep();

    /**
     * @brief Initialize Dijkstra's algorithm
     */
    void initialize();

    /**
     * @brief Find unvisited node with minimum distance
     */
    std::string findMinUnvisited();

    /**
     * @brief Update distances for neighbors of current node
     */
    void updateNeighbors(const std::string& currentNode);
};
