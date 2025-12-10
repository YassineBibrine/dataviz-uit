// src/algorithms/dfs_runner.cpp
#include "dfs_runner.h"
#include <iostream>
#include <map>
#include <algorithm>

DFSRunner::DFSRunner(Graph* graph, const std::string& startNodeId)
    : graph(graph), startNodeId(startNodeId), isExecuted(false) {
    if (!graph || !graph->hasNode(startNodeId)) {
        std::cerr << "Error: Invalid graph or start node" << std::endl;
    }
}

void DFSRunner::initialize() {
    visited.clear();
    while (!stack.empty()) stack.pop();
    traversalOrder.clear();
    discoveredEdges.clear();
    currentStep = 0;
    isExecuted = false;

    if (graph && graph->hasNode(startNodeId)) {
        stack.push(startNodeId);
        
        // Record initial state
        std::map<std::string, std::string> details;
        details["node"] = startNodeId;
        details["operation"] = "initialize";
        executionState.recordOperation("initialize", details);
    }
}

void DFSRunner::execute() {
    if (!graph) return;

    initialize();

    while (!stack.empty()) {
        processStep();
    }

    isExecuted = true;

    // Final metrics
    std::map<std::string, int> metrics = executionState.getMetrics();
    std::cout << "DFS Traversal completed. Visited nodes: " << visited.size() << std::endl;
}

void DFSRunner::processStep() {
    if (stack.empty() || !graph) return;

    // Peek at top of stack
    std::string currentNode = stack.top();
    stack.pop();

    // Check if already visited
    if (visited.find(currentNode) != visited.end()) {
        return;
    }

    // Mark as visited
    visited.insert(currentNode);
    traversalOrder.push_back(currentNode);

    // Record access
    std::map<std::string, std::string> accessDetails;
    accessDetails["node"] = currentNode;
    executionState.recordOperation("access", accessDetails);

    // Get neighbors
    std::vector<std::string> neighbors = graph->getNeighbors(currentNode);

    // Push unvisited neighbors to stack (in reverse order to maintain left-to-right traversal)
    for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
        const auto& neighbor = *it;

        // Record comparison
        std::map<std::string, std::string> comparisonDetails;
        comparisonDetails["current"] = currentNode;
        comparisonDetails["neighbor"] = neighbor;
        executionState.recordOperation("comparison", comparisonDetails);

        // Check if visited
        if (visited.find(neighbor) == visited.end()) {
            stack.push(neighbor);
            discoveredEdges.push_back({currentNode, neighbor});

            // Record discovery
            std::map<std::string, std::string> discoveryDetails;
            discoveryDetails["from"] = currentNode;
            discoveryDetails["to"] = neighbor;
            discoveryDetails["operation"] = "discover";
            executionState.recordOperation("discover", discoveryDetails);
        }
    }

    executionState.advanceStep();
    currentStep++;
}

void DFSRunner::stepForward() {
    if (!graph || isExecuted) return;

    if (stack.empty()) {
        isExecuted = true;
        return;
    }

    processStep();
}

void DFSRunner::stepBackward() {
    // Note: Full backward traversal is complex for DFS.
    // A simple approach: reset and replay up to (currentStep - 1)
    if (currentStep > 0) {
        int targetStep = currentStep - 1;
        reset();
        
        for (int i = 0; i < targetStep && !stack.empty(); ++i) {
            processStep();
        }
    }
}

void DFSRunner::reset() {
    AlgorithmRunner::reset();
    initialize();
}
