#pragma once

#include "algorithm_runner.h"
#include "../core/graph.h"
#include <stack>
#include <set>
#include <vector>

/**
 * @class DFSRunner
 * @brief Depth-First Search algorithm runner for graph traversal
 * Records all steps and metrics for visualization and playback
 */
class DFSRunner : public AlgorithmRunner {
private:
    Graph* graph;
    std::string startNodeId;
    std::set<std::string> visited;
    std::stack<std::string> stack;
    std::vector<std::string> traversalOrder;
    std::vector<std::pair<std::string, std::string>> discoveredEdges;
    bool isExecuted{false};

public:
    DFSRunner(Graph* graph, const std::string& startNodeId);
    ~DFSRunner() override = default;

    /**
     * @brief Execute full DFS traversal
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
     * @brief Reset the DFS state
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
     * @brief Process one step of DFS
     */
    void processStep();

    /**
     * @brief Initialize DFS with start node
     */
    void initialize();
};
