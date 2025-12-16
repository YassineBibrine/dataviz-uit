// src/algorithms/dijkstra_runner.cpp
#include "dijkstra_runner.h"
#include <iostream>
#include <iomanip>

DijkstraRunner::DijkstraRunner(Graph* graph, const std::string& startNodeId)
    : graph(graph), startNodeId(startNodeId), isExecuted(false) {
    if (!graph || !graph->hasNode(startNodeId)) {
        std::cerr << "Error: Invalid graph or start node" << std::endl;
    }
}

void DijkstraRunner::initialize() {
    distances.clear();
    previousNode.clear();
    visited.clear();
    unvisited.clear();
    processedOrder.clear();
    shortestPathEdges.clear();
    currentStep = 0;
    isExecuted = false;

    if (!graph) return;

    // Initialize all distances to infinity
    for (const auto& nodeId : graph->getAllNodeIds()) {
        distances[nodeId] = INFINITY_DIST;
        unvisited.insert(nodeId);
    }

    // Start node distance is 0
    if (graph->hasNode(startNodeId)) {
        distances[startNodeId] = 0;
        
        // Record initial state
        std::map<std::string, std::string> details;
        details["node"] = startNodeId;
        details["distance"] = "0";
        details["operation"] = "initialize";
        executionState.recordOperation("initialize", details);
    }
}

void DijkstraRunner::execute() {
    if (!graph) return;

    initialize();

    while (!unvisited.empty()) {
        std::string minNode = findMinUnvisited();
        if (minNode.empty()) break;
        
        processStep();
    }

    isExecuted = true;

    // Final metrics
    std::map<std::string, int> metrics = executionState.getMetrics();
    std::cout << "Dijkstra's algorithm completed. Visited nodes: " << visited.size() << std::endl;
}

std::string DijkstraRunner::findMinUnvisited() {
    std::string minNode;
    double minDist = INFINITY_DIST;

    for (const auto& nodeId : unvisited) {
        if (distances[nodeId] < minDist) {
            minDist = distances[nodeId];
            minNode = nodeId;
        }
    }

    return minNode;
}

void DijkstraRunner::updateNeighbors(const std::string& currentNode) {
    if (!graph) return;

    std::vector<std::string> neighbors = graph->getNeighbors(currentNode);
    double currentDist = distances[currentNode];

    for (const auto& neighbor : neighbors) {
        if (visited.find(neighbor) != visited.end()) {
            continue; // Already visited
        }

        // Get edge weight
        Graph::Edge* edge = graph->getEdge(currentNode, neighbor);
        double edgeWeight = (edge != nullptr) ? edge->weight : 1.0;
        double newDist = currentDist + edgeWeight;

        // Record comparison
        std::map<std::string, std::string> comparisonDetails;
        comparisonDetails["current"] = currentNode;
        comparisonDetails["neighbor"] = neighbor;
        comparisonDetails["currentDist"] = std::to_string(static_cast<int>(currentDist));
        comparisonDetails["newDist"] = std::to_string(static_cast<int>(newDist));
        comparisonDetails["oldDist"] = std::to_string(static_cast<int>(distances[neighbor]));
        executionState.recordOperation("comparison", comparisonDetails);

        // Update if new distance is shorter
        if (newDist < distances[neighbor]) {
            distances[neighbor] = newDist;
            previousNode[neighbor] = currentNode;
            shortestPathEdges.push_back({currentNode, neighbor});

            // Record update
            std::map<std::string, std::string> updateDetails;
            updateDetails["node"] = neighbor;
            updateDetails["oldDist"] = std::to_string(static_cast<int>(distances[neighbor]));
            updateDetails["newDist"] = std::to_string(static_cast<int>(newDist));
            updateDetails["operation"] = "update";
            executionState.recordOperation("update", updateDetails);
        }
    }
}

void DijkstraRunner::processStep() {
    if (unvisited.empty() || !graph) return;

    // Find unvisited node with minimum distance
    std::string currentNode = findMinUnvisited();
    if (currentNode.empty()) return;

    // Mark as visited
    visited.insert(currentNode);
    unvisited.erase(currentNode);
    processedOrder.push_back(currentNode);

    // Record access
    std::map<std::string, std::string> accessDetails;
    accessDetails["node"] = currentNode;
    accessDetails["distance"] = std::to_string(static_cast<int>(distances[currentNode]));
    executionState.recordOperation("access", accessDetails);

    // Update distances to neighbors
    updateNeighbors(currentNode);

    executionState.advanceStep();
    currentStep++;
}

void DijkstraRunner::stepForward() {
    if (!graph || isExecuted) return;

    if (unvisited.empty()) {
        isExecuted = true;
        return;
    }

    processStep();
}

void DijkstraRunner::stepBackward() {
    // Note: Full backward traversal is complex for Dijkstra.
    // A simple approach: reset and replay up to (currentStep - 1)
    if (currentStep > 0) {
        int targetStep = currentStep - 1;
        reset();
        
        for (int i = 0; i < targetStep && !unvisited.empty(); ++i) {
            processStep();
        }
    }
}

void DijkstraRunner::reset() {
    AlgorithmRunner::reset();
    initialize();
}

double DijkstraRunner::getShortestDistance(const std::string& nodeId) const {
    auto it = distances.find(nodeId);
    if (it != distances.end()) {
        return it->second;
    }
    return INFINITY_DIST;
}

std::vector<std::string> DijkstraRunner::getShortestPath(const std::string& destinationId) const {
    std::vector<std::string> path;
    
    if (!graph || !graph->hasNode(destinationId)) {
        return path;
    }

    std::string current = destinationId;

    // Reconstruct path from destination to start
    while (!current.empty()) {
        path.insert(path.begin(), current);
        
        auto it = previousNode.find(current);
        if (it == previousNode.end()) {
            break;
        }
        
        current = it->second;
    }

    // Verify path starts from startNodeId
    if (!path.empty() && path[0] == startNodeId) {
        return path;
    }

    return std::vector<std::string>(); // No path found
}
