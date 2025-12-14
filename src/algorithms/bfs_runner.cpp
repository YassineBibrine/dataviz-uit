// src/algorithms/bfs_runner.cpp
#include "bfs_runner.h"
#include <iostream>
#include <map>

BFSRunner::BFSRunner(Graph* graph, const std::string& startNodeId)
    : graph(graph), startNodeId(startNodeId), isExecuted(false) {
    if (!graph || !graph->hasNode(startNodeId)) {
        std::cerr << "Error: Invalid graph or start node" << std::endl;
    }
}

void BFSRunner::initialize() {
    visited.clear();
    while (!queue.empty()) queue.pop();
    traversalOrder.clear();
    discoveredEdges.clear();
    currentStep = 0;
    isExecuted = false;

    if (graph && graph->hasNode(startNodeId)) {
        queue.push(startNodeId);
        visited.insert(startNodeId);
        traversalOrder.push_back(startNodeId);
        
        // Record initial state
        std::map<std::string, std::string> details;
        details["node"] = startNodeId;
        details["operation"] = "initialize";
        executionState.recordOperation("initialize", details);
    }
}

void BFSRunner::execute() {
    if (!graph) return;

    initialize();

    while (!queue.empty()) {
        processStep();
    }

    isExecuted = true;

    // Final metrics
    std::map<std::string, int> metrics = executionState.getMetrics();
    std::cout << "BFS Traversal completed. Visited nodes: " << visited.size() << std::endl;
}

void BFSRunner::processStep() {
    if (queue.empty() || !graph) return;

    // Dequeue a node
    std::string currentNode = queue.front();
    queue.pop();

    // Record access
    std::map<std::string, std::string> accessDetails;
    accessDetails["node"] = currentNode;
    executionState.recordOperation("access", accessDetails);

    // Get neighbors
    std::vector<std::string> neighbors = graph->getNeighbors(currentNode);

    for (const auto& neighbor : neighbors) {
        // Record comparison
        std::map<std::string, std::string> comparisonDetails;
        comparisonDetails["current"] = currentNode;
        comparisonDetails["neighbor"] = neighbor;
        executionState.recordOperation("comparison", comparisonDetails);

        // Check if visited
        if (visited.find(neighbor) == visited.end()) {
            visited.insert(neighbor);
            queue.push(neighbor);
            traversalOrder.push_back(neighbor);
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

void BFSRunner::stepForward() {
    if (!graph || isExecuted) return;

    if (queue.empty()) {
        isExecuted = true;
        return;
    }

    processStep();
}

void BFSRunner::stepBackward() {
    // Note: Full backward traversal is complex for BFS.
    // A simple approach: reset and replay up to (currentStep - 1)
    if (currentStep > 0) {
        int targetStep = currentStep - 1;
        reset();
        
        for (int i = 0; i < targetStep && !queue.empty(); ++i) {
            processStep();
        }
    }
}

void BFSRunner::reset() {
    AlgorithmRunner::reset();
    initialize();
}
