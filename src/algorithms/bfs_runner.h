#pragma once

#include "algorithm_runner.h"
#include "../core/graph.h"
#include <queue>
#include <set>
#include <vector>

/**
 * @class BFSRunner
 * @brief Breadth-First Search algorithm runner for graph traversal
 * Records all steps and metrics for visualization and playback
 */
class BFSRunner : public AlgorithmRunner {
private:
    Graph* graph;
    std::string startNodeId;
    std::set<std::string> visited;
    std::queue<std::string> queue;
    std::vector<std::string> traversalOrder;
    std::vector<std::pair<std::string, std::string>> discoveredEdges;
    bool isExecuted{false};

public:
    BFSRunner(Graph* graph, const std::string& startNodeId);
    ~BFSRunner() override = default;

    /**
     * @brief Execute full BFS traversal
     */
    void execute() override;

    /**
     * @brief Step forward one node
     */
    void stepForward() override;

    /**
     * @brief Step backward one node
     */
    void stepBackward() override;

    /**
     * @brief Reset the BFS state
     */
    void reset() override;

    /**
     * @brief Get the traversal order
     */
    std::vector<std::string> getTraversalOrder() const { return traversalOrder; }

    /**
     * @brief Get discovered edges during traversal
     */
    std::vector<std::pair<std::string, std::string>> getDiscoveredEdges() const { return discoveredEdges; }

    /**
     * @brief Check if a node has been visited
     */
    bool isNodeVisited(const std::string& nodeId) const { return visited.count(nodeId) > 0; }

private:
    /**
     * @brief Process one step of BFS
     */
    void processStep();

    /**
     * @brief Initialize BFS with start node
     */
    void initialize();
};
